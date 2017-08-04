#include "beamsearch.h"
#include "expandedpolygon.h"
#include "neopolygonviewer.h"
#include "polygonviewer.h"
#include "Utils/polygonconnector.h"
#include "Evaluation/evaluation.h"

#include <thread>
#include <mutex>
#include <condition_variable>
//もしデバックモードにしたければ下をコメントアウト
#define DEBUG_MODE

std::condition_variable cond;

BeamSearch::BeamSearch()
{
    logger = spdlog::get("beamsearch");
}

void BeamSearch::makeNextState(std::vector<procon::NeoField> & fields,std::vector<Evaluate> & evaluations)
{

#ifdef DEBUG_MODE
    std::sort(evaluations.begin(),evaluations.end(),[](Evaluate l,Evaluate r){
        return l.score > r.score;
    });

#elif
#endif
}

bool BeamSearch::checkCanPrune(const procon::NeoField &field)
{
    return false;
}

void BeamSearch::evaluateNextState(std::vector<procon::NeoField> & fields,std::vector<Evaluate> & evaluations)
{
    //frameがstd::vector<NeoExPolygon>なのでそれぞれに対して、評価関数を回す
    auto evaluateWrapper = [&](procon::NeoField const& field,int const& piece_index){
        int frame_index = 0;
        for(const auto& f : field.getFrame()){
            std::vector<std::pair<double,Connect>> evaluate = Evaluation::evaluation(f,field.getElementaryPieces()[piece_index]);

            for(const auto& e : evaluate){
                Evaluate ev_buf;
                ev_buf.score = e.first;
                ev_buf.connection = e.second;
                ev_buf.frame_index = frame_index;
                ev_buf.piece_index = piece_index;
                evaluations.push_back(ev_buf);
            }

            ++frame_index;
        }
    };

    auto evaluateNextState = [&](procon::NeoField const& field){
        for (int piece_index = 0; piece_index < field.getElementaryPieces().size(); ++piece_index) {
            //すでに置いてあったら評価しません
            if(field.getIsPlaced().at(piece_index)) continue;

            evaluateWrapper(field,piece_index);
        }
    };

#ifdef DEBUG_MODE
    for(auto const& f : fields){
        evaluateNextState(f);
    }
#else

#endif
}

void BeamSearch::init()
{
#ifdef DEBUG_MODE
    logger->info("debug mode");
#else
    logger->info("efficient mode");
#endif
    this->processor_num = std::thread::hardware_concurrency();
}

void BeamSearch::run(procon::NeoField field)
{
    logger->info("beamsearch run");

    for (int piece_num = 0; piece_num < static_cast<int>(field.getElementaryPieces().size()); ++piece_num) {

    }

}
