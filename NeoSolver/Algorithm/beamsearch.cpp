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

//std::condition_variable cond;

//PolygonConnector用の
typedef std::tuple<std::vector<procon::NeoExpandedPolygon>, procon::NeoExpandedPolygon, bool> ConnectedResult;

BeamSearch::BeamSearch()
{
    logger = spdlog::get("BeamSearch");
    dock = std::make_shared<NeoAnswerDock>();
    dock->show();
}

void BeamSearch::makeNextState(std::vector<procon::NeoField> & fields,std::vector<Evaluate> & evaluations)
{
    std::vector<procon::NeoField> next_field;

    auto makeNextFieldFromEvaluate = [&](Evaluate eval){
        procon::NeoField field_buf = fields[eval.fields_index];
        ConnectedResult connect_result = PolygonConnector::connect(fields[eval.fields_index].getFrame()[eval.frame_index]
                ,eval.is_inversed
                    ? fields[eval.fields_index].getElementaryPieces()[eval.piece_index]
                    : fields[eval.fields_index].getElementaryInversePieces()[eval.piece_index]
                ,eval.connection
        );

        if(std::get<2>(connect_result)){
            std::array<bool,50> is_placed = field_buf.getIsPlaced();
            is_placed[eval.piece_index] = true;
            field_buf.setIsPlaced(is_placed);

            field_buf.setPiece(std::get<1>(connect_result));

            std::vector<procon::NeoExpandedPolygon> frames_buf = fields[eval.fields_index].getFrame();
            frames_buf.erase(frames_buf.begin() + eval.frame_index);
            for (auto frame_polygon : std::get<0>(connect_result)){
                frames_buf.push_back(frame_polygon);
            }

            field_buf.setFrame(frames_buf);

            next_field.push_back(field_buf);
        }
    };

#ifdef DEBUG_MODE
    std::sort(evaluations.begin(),evaluations.end(),[](Evaluate l,Evaluate r){
        return l.score > r.score;
    });

    for(auto const& eval : evaluations){
        makeNextFieldFromEvaluate(eval);

        //ビーム幅を超えたらああああ終わりです。
        if(next_field.size() == 100){
            break;
        }
    }
    fields.clear();
    std::copy(next_field.begin(),next_field.end(),std::back_inserter(fields));
#elif
#endif
}

bool BeamSearch::checkCanPrune(const procon::NeoField &field)
{
    return false;
    //OKならfalseを返す
}

void BeamSearch::evaluateNextState(std::vector<procon::NeoField> & fields,std::vector<Evaluate> & evaluations)
{
    //frameがstd::vector<NeoExPolygon>なのでそれぞれに対して、評価関数を回す
    auto evaluateWrapper = [&](procon::NeoField const& field,int const& piece_index,int const& fields_index){
        int frame_index = 0;
        for(const auto& f : field.getFrame()){
            //inverseしていない方のpiece評価
            std::vector<std::pair<double,Connect>> evaluate = Evaluation::evaluation(f,field.getElementaryPieces()[piece_index]);
            //inverseしている方のpiece評価
            std::vector<std::pair<double,Connect>> evaluate_inversed = Evaluation::evaluation(f,field.getElementaryInversePieces()[piece_index]);

            //一時保存用の変数
//            TODO: いい感じにここをパフォーマンスよくする
            Evaluate ev_buf;
            for(const auto& e : evaluate){
                ev_buf.score = e.first;
                ev_buf.connection = e.second;
                ev_buf.fields_index = fields_index;
                ev_buf.frame_index = frame_index;
                ev_buf.piece_index = piece_index;
                ev_buf.is_inversed = false;
                evaluations.push_back(ev_buf);
            }

            for(const auto& e : evaluate_inversed){
                ev_buf.score = e.first;
                ev_buf.connection = e.second;
                ev_buf.fields_index = fields_index;
                ev_buf.frame_index = frame_index;
                ev_buf.piece_index = piece_index;
                ev_buf.is_inversed = true;
                evaluations.push_back(ev_buf);
            }

            ++frame_index;
        }
    };

    auto evaluateNextState = [&](procon::NeoField const& field,int const& fields_index){
        for (int piece_index = 0; piece_index < field.getElementaryPieces().size(); ++piece_index) {
            //すでに置いてあったら評価しません
            if(field.getIsPlaced().at(piece_index)) continue;

            evaluateWrapper(field,piece_index,fields_index);
        }
    };

#ifdef DEBUG_MODE
    int field_index = 0;
    for(auto const& f : fields){
        evaluateNextState(f,field_index);
        ++field_index;
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
    dock->addAnswer(field);

    std::vector<procon::NeoField> state;
    state.push_back(field);

    for (int piece_num = 0; piece_num < static_cast<int>(field.getElementaryPieces().size()); ++piece_num) {
        std::vector<Evaluate> ev;
        evaluateNextState(state,ev);
        makeNextState(state,ev);

        for(auto const& _field : state){
            dock->addAnswer(_field);
        }
        break;
    }
}
