#include "beamsearch.h"

BeamSearch::BeamSearch()
{

}

void BeamSearch::evaluateNextMove (std::vector<Evaluation> & evaluations,std::vector<procon::Field> const& field_vec)
{
    auto fieldSearch = [&](procon::Field field)
    {
        std::vector<Evaluation> evaluations;
        for (int j = 0;j < static_cast<int>(field.getElementaryPieces().size());j++){
            if (field.getIsPlaced().at(j)) continue;
            std::vector<Evaluation> eva = evaluateCombination(field.getFlame(),field.getElementaryPieces().at(j));
            std::vector<Evaluation> eva_inverse = evaluateCombination(field.getFlame(),field.getElementaryInversePieces().at(j));
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

    for (int j = 0;j < static_cast<int>(field_vec.size());j++) {
        std::vector<Evaluation> eva = fieldSearch(field_vec.at(j));
        for (auto & e : eva) {
            e.vector_id = j;
        }
        std::copy(eva.begin(),eva.end(),std::back_inserter(evaluations));
    }

}

std::vector<procon::Field> BeamSearch::makeNextField (std::vector<Evaluation> const& evaluations,std::vector<procon::Field> const& field_vec)
{
    std::vector<procon::Field> next_field_vec;
    for (int j = 0;j < beam_width && j < static_cast<int>(evaluations.size());j++) {
        int const& vec_id = evaluations.at(j).vector_id;
        int const& piece_id = evaluations.at(j).piece_id;
        std::array<Fit,2> const& fits = evaluations.at(j).fits;
        procon::ExpandedPolygon const& old_frame = field_vec.at(vec_id).getFlame();
        procon::ExpandedPolygon old_piece;
        if (evaluations.at(j).inverse_flag){
            old_piece = field_vec.at(vec_id).getElementaryInversePieces().at(piece_id);
        } else {
            old_piece = field_vec.at(vec_id).getElementaryPieces().at(piece_id);
        }
        procon::ExpandedPolygon new_frame;
        procon::Field new_field = field_vec.at(vec_id);

        bool hasJoinSuccess = PolygonConnector::joinPolygon(old_frame,old_piece,new_frame,fits);

        if (hasJoinSuccess) {
            new_field.setFlame(new_frame);
            new_field.setIsPlaced(piece_id);
            next_field_vec.push_back(std::move(new_field));
        }
    }
    return next_field_vec;
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
