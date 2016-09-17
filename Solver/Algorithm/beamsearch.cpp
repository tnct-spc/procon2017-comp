#include "beamsearch.h"

BeamSearch::BeamSearch()
{

}

std::vector<Evaluation> BeamSearch::fieldSearch(procon::Field field)
{
    std::vector<Evaluation> evaluations;
    for (int j = 0;j < static_cast<int>(field.getElementaryPieces().size());j++){
        if (field.getIsPlaced().at(j)) continue;
        std::vector<Evaluation> eva = evaluateCombination(field.getFlame(),field.getElementaryPieces().at(j));
        for (auto & e : eva){
            e.piece_id = j;
        }
        std::copy(eva.begin(),eva.end(),std::back_inserter(evaluations));
    }
    return evaluations;
}

procon::Field BeamSearch::run(procon::Field field)
{
    constexpr double beam_width = 10;
    auto sortEvaLambda = [&](Evaluation const& a,Evaluation const& b)->bool {
        return a.evaluation > b.evaluation;
    };

    std::vector<procon::Field> field_vec;
    std::vector<procon::Field> next_field_vec;
    field.setFlame(field.getElementaryFlame());
    field_vec.push_back(field);
    procon::Field buckup_field;

    //ピースが全部置かれたら終了
    for (int i = 0;i < static_cast<int>(field.getElementaryPieces().size());i++){

        buckup_field = field_vec.at(0);
        std::vector<Evaluation> evaluations;

        for (int j = 0;j < static_cast<int>(field_vec.size());j++) {
            std::vector<Evaluation> eva = fieldSearch(field_vec.at(j));
            for (auto & e : eva){
                e.vector_id = j;
            }
            std::copy(eva.begin(),eva.end(),std::back_inserter(evaluations));
        }

        //それより先がなければその1手前の最高評価値のフィールドを返す
        if (evaluations.size() == 0) return buckup_field;

        sort(evaluations.begin(),evaluations.end(),sortEvaLambda);

        for (int j = 0;j < beam_width && j < static_cast<int>(evaluations.size());j++) {
            procon::ExpandedPolygon new_frame;
            const int vec_id = evaluations.at(j).vector_id;
            const int piece_id = evaluations.at(j).piece_id;
            procon::Field new_field = field_vec.at(vec_id);
            bool hasJoinSuccess = PolygonConnector::joinPolygon(field_vec.at(vec_id).getFlame(),field_vec.at(vec_id).getElementaryPieces().at(piece_id),new_frame,evaluations.at(j).fits);
            if(!hasJoinSuccess) continue;
            new_field.setFlame(new_frame);
            next_field_vec.push_back(std::move(new_field));
        }
        field_vec = std::move(next_field_vec);
        if(field_vec.empty()) return buckup_field;
    }
    return field_vec.at(0);
    /*
    procon::ExpandedPolygon ret_expol;
    procon::Field ret_field;
    std::array<Fit,2> fits;
    fits.at(0).start_id = 5;
    fits.at(0).end_id = 5;
    fits.at(0).flame_inner_pos = 0;
    fits.at(0).start_dot_or_line = Fit::Dot;
    fits.at(0).end_dot_or_line = Fit::Dot;
    fits.at(1).start_id = 1;
    fits.at(1).end_id = 1;
    fits.at(1).start_dot_or_line = Fit::Dot;
    fits.at(1).end_dot_or_line = Fit::Dot;
    fits.at(1).flame_inner_pos = -1;
    PolygonConnector::joinPolygon(field.getFlame(),field.getElementaryPieces().at(13),ret_expol,fits);
    ret_field = field;
    ret_field.setFlame(ret_expol);
    return ret_field;
    */
}
