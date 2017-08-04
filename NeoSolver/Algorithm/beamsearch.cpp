#include "beamsearch.h"
#include "expandedpolygon.h"
#include "neopolygonviewer.h"
#include "polygonviewer.h"
#include "Utils/polygonconnector.h"
#include "Evaluation/evaluation.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <boost/geometry.hpp>
#include <boost/geometry/multi/multi.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/multi/geometries/multi_polygon.hpp>
#include <boost/geometry/io/wkt/wkt.hpp>
#include <boost/geometry/multi/io/wkt/wkt.hpp>

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
    //OKならfalseを返す
    int piecesize = field.getPiece.getSize();
    int fieldsize;
    bg::num_points(field.getFrame(), fieldsize);
    double hoge_angle;
    double min_angle = 360;
    for(int a = 0; a < piecesize; ++piecesize){
        hoge_angle = field.getPiece().getSideAngle().at(piecesize);
        if(hoge_angle < min_angle) min_angle = hoge_angle;
    }
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
