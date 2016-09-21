#include "beamsearch.h"

#include "field.h"
#include "Utils/evaluation.h"
#include "Utils/polygonconnector.h"

BeamSearch::BeamSearch()
{

}

void BeamSearch::evaluateNextMove (std::vector<Evaluation> & evaluations,std::vector<procon::Field> const& field_vec)
{
    auto fieldSearch = [&](procon::Field const& field)
    {
        std::vector<Evaluation> evaluations;
        for (int j = 0;j < static_cast<int>(field.getElementaryPieces().size());j++){
            if (field.getIsPlaced().at(j)) continue;
            std::vector<Evaluation> eva = evaluateCombinationByAngle(field.getFlame(),field.getElementaryPieces().at(j));
            std::vector<Evaluation> eva_inverse = evaluateCombinationByAngle(field.getFlame(),field.getElementaryInversePieces().at(j));
            for (auto & e : eva){
                e.piece_id = j;
            }
            for (auto & e : eva_inverse) {
                e.piece_id = j;
                e.inverse_flag = true;
            }
            std::copy(eva.begin(),eva.end(),std::back_inserter(evaluations));
            std::copy(eva_inverse.begin(),eva_inverse.end(),std::back_inserter(evaluations));
        }
        return evaluations;
    };

    tbb::task_scheduler_init tbb;
    tbb::concurrent_vector<Evaluation> tbb_evaluations;
    tbb::concurrent_vector<procon::Field> tbb_field_vec;

    for (auto const& field : field_vec) {
        tbb_field_vec.push_back(field);
    }

    int const field_vec_size = static_cast<int>(field_vec.size());

    auto evaluateRange = [&](tbb::blocked_range<int> const& range)
    {
        for (auto j = range.begin();j < range.end();j++) {
            std::vector<Evaluation> eva = fieldSearch(tbb_field_vec.at(j));
            for (auto & e : eva) {
                e.vector_id = j;
            }
            std::copy(eva.begin(),eva.end(),std::back_inserter(tbb_evaluations));
        }
    };

    tbb::parallel_for(tbb::blocked_range<int>(0,field_vec_size,(field_vec_size / 4) + 1),evaluateRange);

    for (auto & eva : tbb_evaluations) {
        evaluations.emplace_back(eva);
    }

    tbb.terminate();
}

std::vector<procon::Field> BeamSearch::makeNextField (std::vector<Evaluation> const& evaluations,std::vector<procon::Field> const& field_vec)
{
    std::vector<procon::Field> next_field_vec;
    for (int j = 0;j < beam_width && j < static_cast<int>(evaluations.size());j++) {
        int const& vec_id = evaluations.at(j).vector_id;
        int const& piece_id = evaluations.at(j).piece_id;
        std::array<Fit,2> const& fits = evaluations.at(j).fits;
        procon::ExpandedPolygon const& old_frame = field_vec.at(vec_id).getFlame();
        procon::ExpandedPolygon const& old_piece =
        (evaluations.at(j).inverse_flag) ?
            field_vec.at(vec_id).getElementaryInversePieces().at(piece_id)
        :
            field_vec.at(vec_id).getElementaryPieces().at(piece_id)
        ;
        procon::ExpandedPolygon new_frame;

        bool hasJoinSuccess = PolygonConnector::joinPolygon(old_frame,old_piece,new_frame,fits);
        double const min_angle = field_vec.at(vec_id).getMinAngle();

        if (hasJoinSuccess  && !canPrune(new_frame,min_angle) ) {
            procon::Field new_field = field_vec.at(vec_id);
            new_field.setFlame(new_frame);
            new_field.setIsPlaced(piece_id);
            next_field_vec.emplace_back(new_field);
        }
    }
    return next_field_vec;
}

bool BeamSearch::canPrune(procon::ExpandedPolygon const& next_frame ,double const& min_angle) {
    bool can_prune = false;
    for (auto const& angles : next_frame.getInnersSideAngle()) {
        for (auto const& angle : angles){
            if (angle < min_angle) {
                can_prune = true;
            }
        }
    }
    return  can_prune;
}

procon::Field BeamSearch::run(procon::Field field)
{
    auto sortEvaLambda = [&](Evaluation const& a,Evaluation const& b)->bool
    {
        return a.evaluation > b.evaluation;
    };

    std::vector<procon::Field> field_vec;
    std::vector<Evaluation> evaluations;

    field.setFlame(field.getElementaryFlame());
    field_vec.push_back(field);
    procon::Field buckup_field;

    //ピースが全部置かれたら終了
    //このiは添字として使ってるわけではない（ただの回数ループ）
    for (int i = 0;i < static_cast<int>(field.getElementaryPieces().size());i++){
        evaluations.clear();
        for (int j = 0;j < static_cast<int>(field_vec.size());j++){
            field_vec.at(j).calcMinAngleSide();
        }
        buckup_field = field_vec.at(0);
        this->evaluateNextMove(evaluations,field_vec);
        //それより先がなければその1手前の最高評価値のフィールドを返す
        if (evaluations.empty()) return buckup_field;

        sort(evaluations.begin(),evaluations.end(),sortEvaLambda);
        field_vec = std::move(this->makeNextField(evaluations,field_vec));

        //結合できるものがなければその１手前の最高評価地のフィールドを返す
        if(field_vec.empty()) return buckup_field;
    }
    return field_vec.at(0);
}
