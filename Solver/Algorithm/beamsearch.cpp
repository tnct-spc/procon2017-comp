#include "beamsearch.h"
#include "field.h"
#include "Utils/evaluation.h"
#include "Utils/polygonconnector.h"
#include <mutex>
#include <thread>
#include "parallel.h"

BeamSearch::BeamSearch()
{
    this->initialization();
}

void BeamSearch::initialization()
{
    cpu_num = std::thread::hardware_concurrency();
}

void BeamSearch::evaluateNextMove (std::vector<Evaluation> & evaluations,std::vector<procon::Field> const& field_vec)
{
    auto fieldSearch = [&](procon::Field const& field)
    {
        std::vector<Evaluation> evaluations;
        for (int j = 0;j < static_cast<int>(field.getElementaryPieces().size());j++){
            if (field.getIsPlaced().at(j)) continue;
            std::vector<Evaluation> eva = evaluateCombinationByAngle(field.getFrame(),field.getElementaryPieces().at(j));
            std::vector<Evaluation> eva_inverse = evaluateCombinationByAngle(field.getFrame(),field.getElementaryInversePieces().at(j));
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

    procon::Parallel parallel;

    int const field_vec_size = static_cast<int>(field_vec.size());

    auto evaluateRange = [&](int start_id,int end_id)
    {
        for (int j = start_id;j < end_id;j++) {
            std::vector<Evaluation> eva = fieldSearch(field_vec.at(j));
            for (auto & e : eva) {
                e.vector_id = j;
            }
            {
                //std::lock_guard<std::mutex> ld(parallel.mutex());と同じ
                MUTEX_LOCK(parallel);
                std::copy(eva.begin(),eva.end(),std::back_inserter(evaluations));
            }
        }
    };

    /**cpuのスレッド数に合わせてvectorを分割し，それぞれスレッドに投げ込む**/
    parallel.generateThreads(evaluateRange,cpu_num,0,field_vec_size);
    /**スレッド終わるの待ち**/
    parallel.joinThreads();

}

std::vector<procon::Field> BeamSearch::makeNextField (std::vector<Evaluation> const& evaluations,std::vector<procon::Field> const& field_vec)
{
    std::vector<procon::Field> next_field_vec;
    procon::Parallel parallel;

    auto makeField = [&](int start_id,int end_id){
        for (int j = start_id;j < end_id;j++) {
            int const vec_id = evaluations.at(j).vector_id;
            int const piece_id = evaluations.at(j).piece_id;
            std::array<Fit,2> const fits = evaluations.at(j).fits;
            procon::ExpandedPolygon const old_frame = field_vec.at(vec_id).getFrame();
            procon::ExpandedPolygon const old_piece =
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
                new_field.setFrame(new_frame);
                new_field.setIsPlaced(piece_id);

                {
                    MUTEX_LOCK(parallel);
                    next_field_vec.push_back(new_field);
                }
            }
        }
    };

    int const width = beam_width < static_cast<int>(evaluations.size()) ? beam_width : static_cast<int>(evaluations.size());
    /**cpuのスレッド数に合わせてvectorを分割し，それぞれスレッドに投げ込む**/
    parallel.generateThreads(makeField,cpu_num,0,width);

    /**スレッド終わるの待ち**/
    parallel.joinThreads();
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

    field.setFrame(field.getElementaryFrame());
    field_vec.push_back(field);
    procon::Field buckup_field;

    //ピースが全部置かれたら終了
    //このiは添字として使ってるわけではない（ただの回数ルーブ）
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
        //return field_vec[0];

        //結合できるものがなければその１手前の最高評価地のフィールドを返す
        if(field_vec.empty()) return buckup_field;
    }
    return field_vec.at(0);
}
