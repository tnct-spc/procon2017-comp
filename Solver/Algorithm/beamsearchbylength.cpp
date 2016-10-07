#include "beamsearchbylength.h"
#include "parallel.h"

//#define NO_PARALLEL

BeamSearchByLength::BeamSearchByLength()
{

}

void BeamSearchByLength::evaluateNextMove (std::vector<Evaluation> & evaluations,std::vector<procon::Field> const& field_vec)
{
    auto fieldSearch = [&](procon::Field const& field)
    {
        std::vector<Evaluation> evaluations;
        for (int j = 0;j < static_cast<int>(field.getElementaryPieces().size());j++){
            if (field.getIsPlaced().at(j)) continue;
            std::vector<Evaluation> eva = evaluateCombinationByLength(field.getFrame(),field.getElementaryPieces().at(j));
            std::vector<Evaluation> eva_inverse = evaluateCombinationByLength(field.getFrame(),field.getElementaryInversePieces().at(j));
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

#ifndef NO_PARALLEL
    /**cpuのスレッド数に合わせてvectorを分割し，それぞれスレッドに投げ込む**/
    parallel.generateThreads(evaluateRange,cpu_num,0,field_vec_size);
    /**スレッド終わるの待ち**/
    parallel.joinThreads();
#else
    evaluateRange(0,field_vec_size);
#endif

}
