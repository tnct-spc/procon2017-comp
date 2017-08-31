#include "beamsearch.h"
#include "expandedpolygon.h"
#include "neopolygonviewer.h"
#include "polygonviewer.h"
#include "Utils/polygonconnector.h"
#include "Evaluation/evaluation.h"

#include <math.h>
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
//    logger = spdlog::get("beamsearch");
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
    //角について枝切りできるかできないか
    auto about_angle = [&field](){

        return false;

        double hoge_frame_angle;
        double min_frame_angle = 360;
        for(auto const &frame : field.getFrame()){
            for(int angle = 0; angle < frame.getSize(); ++angle){
                hoge_frame_angle = frame.getSideAngle().at(angle);
                if(hoge_frame_angle < min_frame_angle) min_frame_angle = hoge_frame_angle;
            }
        }

        double hoge_piece_angle;
        double min_piece_angle = 360;
        for(auto const &piece : field.getPieces()){
            for(int pieceangle = 0; pieceangle < piece.getSize(); ++pieceangle){
                hoge_piece_angle = piece.getSideAngle().at(pieceangle);
                if(hoge_piece_angle < min_piece_angle) min_piece_angle = hoge_piece_angle;
            }
        }

        if(min_frame_angle < min_piece_angle){
            return true;
        }else{
            return false;
        }
    };

    auto framesize_single = [&field](procon::NeoExpandedPolygon frame){//一つのフレームとピースとの面積が合致するかを出す関数
        std::vector<double> area_vec;
        const int frame_area = bg::area(frame.getPolygon());
        std::cout << field.getElementaryPieces().size() << field.getFrame().size() << std::endl;
        for(auto piece : field.getElementaryPieces()){
            int area = bg::area(piece.getPolygon());
            std::cout << "frame_area : " << frame_area << "  piece_area : " << area << std::endl;
            if(area < frame_area)area_vec.push_back(area);//面積を片っ端から代入
            else if(area == frame_area)return false;//このframeに関しては問題ない
        }
        std::cout << "area_vec.size : " << area_vec.size() << std::endl;
        if(area_vec.size() == 0)return true;//問題があるのでtrue返して終了
        std::sort(area_vec.begin(),area_vec.end());


        std::vector<double> add_vec = area_vec;
        for(unsigned int count = 1;count < add_vec.size();++count){
            std::cout << "area_vec.size : " << area_vec.size() << "   add_vec.size : " << add_vec.size() << std::endl;
            for(auto area : area_vec){
                for(unsigned int vec_count=0;vec_count<add_vec.size();++vec_count){
                    std::cout << "どうでしょう" << vec_count << std::endl;
                    int add_cou = area + add_vec.at(vec_count);
                    if(add_cou < frame_area)add_vec.push_back(add_cou);
                    else if(add_cou == frame_area){

                        std::cout << "add_vec一覧表示 : ";
                        for(auto count : add_vec){
                            std::cout << count << " ";
                        }
                        std::cout << std::endl;
                        return false;
                    }
                    else break;
                }
            std::sort(add_vec.begin(),add_vec.end());
            add_vec.erase(std::unique(add_vec.begin(),add_vec.end()) , add_vec.end());
            }
        }
        return true;
    };

    //複数のFrameがあるときにピースと面積が合致するか
    auto about_framesize = [&field,&framesize_single](){
        const int frame_size_max = 2000;//これより大きい面積のframeは判定しない(処理に時間がかかるため)
        std::cout << "frame_size : " << field.getFrame().size() << std::endl;
        for(auto frame : field.getFrame()){
            if(bg::area(frame.getPolygon()) < frame_size_max){
                if(framesize_single(frame)){
                    std::cout << "問題の原因になったframe : " << bg::dsv(frame.getPolygon()) << std::endl;
                    return true;
                }
            }
        }
        return false;
    };


    //辺について枝きりできるかできないか
    auto about_side = [&field](){
        //要素数の足し算、引き算
        auto calculation_nep = [](const procon::NeoExpandedPolygon &nep , int index , int cal){
            index = index + cal;
            index = index % nep.getSize();
            if(index < 0){
                index = nep.getSize() + index;
            }
            return index;
        };
        auto calculation_rad = [](double a){
            //ラジアン(radian)から角度(degree)に変換
            return a * 180.0 / 3.141592653589793;
        };

        //frameの辺を大きい順にならべる
        std::vector<std::tuple<double , double , double>> frame_sides;
        for(procon::NeoExpandedPolygon frame : field.getFrame()){
            for(int i = 0 ;i < frame.getSize() ; i++){
                frame_sides.push_back(std::tuple<double , double , double>(
                                          frame.getSideLength().at(i),
                                          frame.getSideAngle().at(i),
                                          frame.getSideAngle().at(calculation_nep(frame,i,1))
                                          )
                                 );
            }
        }
        sort(frame_sides.begin() , frame_sides.end() , [](auto a , auto b){
            if(std::get<0>(a) != std::get<0>(b)){
                return (std::get<0>(a) > std::get<0>(b));
            }else if(std::get<1>(a) != std::get<1>(b)){
                return (std::get<1>(a) > std::get<1>(b));
            }else{
                return std::get<2>(a) > std::get<2>(b);
            }
        });
        for(std::tuple<double , double , double> i : frame_sides){
            std::cout << std::get<0>(i) << " " << calculation_rad(std::get<1>(i))<<" "<<calculation_rad(std::get<2>(i))<<std::endl;
        }

        std::cout<<std::endl;

        //piecesの辺を小さい順にならべる
        std::vector<double> pieces_sides;
        for(int i = 0 ; i < field.getElementaryPieces().size() ; i++){
            if(field.getIsPlaced().at(i)) continue;
            procon::NeoExpandedPolygon piece = field.getElementaryPieces().at(i);
            for(int i = 0 ; i < piece.getSize() ; i++){
                pieces_sides.push_back(piece.getSideLength().at(i));
            }
        }
        sort(pieces_sides.begin() , pieces_sides.end() , [](auto a , auto b){
            return (a < b);
        });
        for(double i : pieces_sides){
            std::cout << i << std::endl;
        }

        bool size_only = std::get<0>(frame_sides.at(0)) < pieces_sides.at(0);
        bool angles = (std::get<1>(frame_sides.at(0)) < M_PI) || (std::get<2>(frame_sides.at(0)) < M_PI);
        return size_only && angles;
    };

    bool a = about_angle();
    bool b = about_side();
    bool c = about_framesize();
    return c;
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
//    logger->info("debug mode");
#else
    logger->info("efficient mode");
#endif
    this->processor_num = std::thread::hardware_concurrency();
}

void BeamSearch::run(procon::NeoField field)
{
//    logger->info("beamsearch run");

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
