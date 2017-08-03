#include "beamsearch.h"
#include "expandedpolygon.h"
#include "neopolygonviewer.h"
#include "polygonviewer.h"
#include "Utils/polygonconnector.h"
#include "Evaluation/evaluation.h"

//もしデバックモードにしたければ下をコメントアウト
#define DEBUG_MODE

BeamSearch::BeamSearch()
{
    logger = spdlog::get("beamsearch");
}

void BeamSearch::makeNextState(std::vector<procon::NeoField> field)
{

}

void BeamSearch::evaluateNextState(std::vector<procon::NeoField> & fields,std::vector<Evaluate> evaluations)
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
    std::cout << "called beamsearch" << std::endl;
    boost::geometry::equals(point_t(2.0,2.0),point_t(2.0,2.0));
    boost::geometry::math::equals(1.0,2.0);

}
