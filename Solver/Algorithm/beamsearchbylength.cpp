#include "beamsearchbylength.h"

BeamSearchByLength::BeamSearchByLength()
{

}

void BeamSearchByLength::evaluateNextMove (std::vector<Evaluation> & evaluations,std::vector<procon::Field> const& field_vec)
{
    auto fieldSearch = [&](procon::Field field)
    {
        std::vector<Evaluation> evaluations;
        for (int j = 0;j < static_cast<int>(field.getElementaryPieces().size());j++){
            if (field.getIsPlaced().at(j)) continue;
            std::vector<Evaluation> eva = evaluateCombinationByLength(field.getFlame(),field.getElementaryPieces().at(j));
            std::vector<Evaluation> eva_inverse = evaluateCombinationByLength(field.getFlame(),field.getElementaryInversePieces().at(j));
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
