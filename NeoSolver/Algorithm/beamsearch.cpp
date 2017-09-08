#include "beamsearch.h"
#include "expandedpolygon.h"
#include "neopolygonviewer.h"
#include "polygonviewer.h"
#include "Utils/polygonconnector.h"
#include "Evaluation/evaluation.h"
#include "parallel.h"

#include <math.h>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <tbb/task_scheduler_init.h>
#include <tbb/tick_count.h>
#include <boost/geometry.hpp>
#include <boost/geometry/multi/multi.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/multi/geometries/multi_polygon.hpp>
#include <boost/geometry/io/wkt/wkt.hpp>
#include <boost/geometry/multi/io/wkt/wkt.hpp>

//もしデバックモードにしたければ下をコメントアウト
//#define DEBUG_MODE

//同じ状態のフレームの削除
#define USE_DUPLICATE_FRAME_PRUNE

//std::condition_variable cond;
std::mutex mtx;

//PolygonConnector用の
typedef std::tuple<std::vector<procon::NeoExpandedPolygon>, procon::NeoExpandedPolygon, bool> ConnectedResult;

void connect_polygon(procon::NeoExpandedPolygon frame, procon::NeoExpandedPolygon connecting_polygon,Connect connection)
{
    NeoPolygonViewer::getInstance().displayPolygon(frame.getPolygon(),"hogehoge",false);
    NeoPolygonViewer::getInstance().displayPolygon(connecting_polygon.getPolygon(),"hogehoge",false);

	//まずくっつくべきpolygon点を原点に持ってきて回転の中心にする
    boost::geometry::strategy::transform::translate_transformer<int,2,2> transform_connect_point_to_origin(
        -connecting_polygon.getPolygon().outer()[connection.polygon_point_index].x(),
        -connecting_polygon.getPolygon().outer()[connection.polygon_point_index].y()
    );
    polygon_i result;
    boost::geometry::transform(connecting_polygon.getPolygon(),result,transform_connect_point_to_origin);
    
    std::pair<int,int> piece_vec = std::make_pair(
        connecting_polygon.getPolygon().outer()[connection.polygon_side_index + 1].x() - connecting_polygon.getPolygon().outer()[connection.polygon_side_index].x(),
        connecting_polygon.getPolygon().outer()[connection.polygon_side_index + 1].y() - connecting_polygon.getPolygon().outer()[connection.polygon_side_index].y()
    );
    std::pair<int,int> frame_vec = std::make_pair(
        frame.getPolygon().outer()[connection.frame_side_index + 1].x() - frame.getPolygon().outer()[connection.frame_side_index].x(),
        frame.getPolygon().outer()[connection.frame_side_index + 1].y() - frame.getPolygon().outer()[connection.frame_side_index].y()
    );
    
    double piece_angle = std::atan2(piece_vec.first,piece_vec.second);
    double frame_angle = std::atan2(frame_vec.first,frame_vec.second);
   
    polygon_i result2;
    polygon_t result2_t;
    boost::geometry::strategy::transform::rotate_transformer<bg::radian,double,2,2> rotate_1(-(piece_angle - frame_angle));
    boost::geometry::strategy::transform::rotate_transformer<bg::radian,double,2,2> rotate_2(-(piece_angle - frame_angle));

    std::cout << "hogehoge" << -(piece_angle - frame_angle) << std::endl;
    std::cout << "hogehoge" << boost::geometry::transform(result,result2,rotate_1) << std::endl;
    std::cout << "hogehoge" << boost::geometry::transform(result,result2_t,rotate_2) << std::endl;
    
    std::cout << "hogehogedksljflksdajklfhja" << boost::geometry::dsv(result2) << std::endl;
    std::cout << "hogehogedksljflksdajklfhja" << boost::geometry::dsv(result2_t) << std::endl;

    procon::ExpandedPolygon pol(connecting_polygon.getId());
    pol.resetPolygonForce(result2_t);
    PolygonViewer::getInstance().pushPolygon(pol,"kkk");
    NeoPolygonViewer::getInstance().displayPolygon(result2,"jjsg",false);
    
    boost::geometry::strategy::transform::translate_transformer<int,2,2> transform_to_connect_point(
        frame.getPolygon().outer()[connection.frame_point_index].x(),
        frame.getPolygon().outer()[connection.frame_point_index].y()
    );
    polygon_i result3;
    boost::geometry::transform(result2,result3);

}

void test()
{
    polygon_i sample1,sample2;
    boost::geometry::exterior_ring(sample1) = boost::assign::list_of<point_i>(0,0)(0,10)(10,10)(10,0)(0,0);
    boost::geometry::exterior_ring(sample2) = boost::assign::list_of<point_i>(15,15)(15,20)(20,15)(15,15);
    Connect con = { 1,1,1,1 };

    polygon_i sample_extra_rotate_polygon;
//    polygon_i sample_answer;
    polygon_t sample_answer;
    polygon_i sampleanswer;
    boost::geometry::exterior_ring(sample_extra_rotate_polygon) = boost::assign::list_of<point_i>(0,0)(0,5)(5,5)(5,0)(0,0);
    boost::geometry::strategy::transform::rotate_transformer<bg::radian,double,2,2> rotate(std::atan2(3,4));
    boost::geometry::strategy::transform::rotate_transformer<bg::radian,double,2,2> rotate2(std::atan2(3,4));
    boost::geometry::transform(sample_extra_rotate_polygon,sample_answer,rotate);
    boost::geometry::transform(sample_extra_rotate_polygon,sampleanswer,rotate2);

//    NeoPolygonViewer::getInstance().displayPolygon(sampleanswer,"hgeoa",false);

    std::cout << std::atan2(3,4) << std::endl;

//    NeoPolygonViewer::getInstance().displayPolygon(sample_extra_rotate_polygon,"before",false);
//    NeoPolygonViewer::getInstance().displayPolygon(sample_answer,"after",false);
    procon::ExpandedPolygon po;
    po.resetPolygonForce(sample_answer);
//    PolygonViewer::getInstance().pushPolygon(po,"hgoehoge");

    std::cout << boost::geometry::dsv(sample_extra_rotate_polygon) << std::endl;
    std::cout << boost::geometry::dsv(sample_answer) << std::endl;

//    connect_polygon(sample1,sample2,con);


    polygon_i frame,piece;
    boost::geometry::exterior_ring(frame) = boost::assign::list_of<point_i>(0,8)(6,0)(14,6)(8,14)(0,8);
    boost::geometry::exterior_ring(piece) = boost::assign::list_of<point_i>(0,0)(0,5)(5,5)(5,0)(0,0);
    procon::NeoExpandedPolygon frameg,pieceg;
    frameg.resetPolygonForce(frame);
    pieceg.resetPolygonForce(piece);
    connect_polygon(frameg,pieceg,con);

}

BeamSearch::BeamSearch()
{
    logger = spdlog::get("BeamSearch");
//    logger = spdlog::get("beamsearch");
    dock = std::make_shared<NeoAnswerDock>();
    dock->show();

    cpu_num = std::thread::hardware_concurrency();
}

std::string BeamSearch::hashField(const procon::NeoField& field){
    std::vector<procon::NeoExpandedPolygon> polygons = field.getFrame();
    std::vector<procon::NeoExpandedPolygon> frame = field.getElementaryFrame();
    //polygonのvectorをすべてpointのvectorにまとめる
    auto polygons_to_points=[](std::vector<procon::NeoExpandedPolygon> vectorA){
        std::vector<point_i> v;
        for(procon::NeoExpandedPolygon i : vectorA){
            polygon_i polygon1 = i.getPolygon();
            polygon1.outer().pop_back();
            for(point_i j : polygon1.outer()){
                v.push_back(j);
            }
        }
        return v;
    };
    //pointのvectorをソートする
    auto sort_points=[](std::vector<point_i> vectorA){
        sort(vectorA.begin(),
             vectorA.end(),
             [](point_i a,point_i b){
                if(a.x()==b.x()){
                    return (a.y() < b.y());
                }
                return (a.x() < b.x());
             }
        );
        return vectorA;
    };
    //int型10進数をstring型16進数に変換する
    auto int_to_string=[](unsigned int val,bool lower = true){
        if( !val )
                return std::string("0");
            std::string str;
            const char hc = lower ? 'a' : 'A';     // 小文字 or 大文字表記
            while( val != 0 ) {
                int d = val & 15;     // 16進数一桁を取得
                if( d < 10 )
                    str.insert(str.begin(), d + '0');  //  10未満の場合
                else //  10以上の場合
                    str.insert(str.begin(), d - 10 + hc);
                val >>= 4;
            }
            return str;
    };
    //pointのvectorを文字列に変換する
    auto points_to_string=[int_to_string](std::vector<point_i> vectorA){
        std::string string;
        for(point_i i : vectorA){
            string+=int_to_string(i.x());
            string+=int_to_string(i.y());
        }
        return string;
    };

    //polygonのvectorをpointにしてソートする
    std::vector<point_i> polygons_points_vector=polygons_to_points(polygons);
    polygons_points_vector=sort_points(polygons_points_vector);

    //frameのouterのpointをソートする
    std::vector<point_i> frame_points_vector;
    for(procon::NeoExpandedPolygon i : frame){
        polygon_i polygon1 = i.getPolygon();
        for(point_i j : polygon1.outer()){
            frame_points_vector.push_back(j);
        }
    }
    frame_points_vector=sort_points(frame_points_vector);

    std::string answer;
    answer+=points_to_string(polygons_points_vector);
    answer+="_";
    answer+=points_to_string(frame_points_vector);
    return answer;
}

void BeamSearch::makeNextState(std::vector<procon::NeoField> & fields,std::vector<Evaluate> & evaluations)
{
    std::vector<procon::NeoField> next_field;

#ifdef DEBUG_MODE
    auto makeNextFieldFromEvaluate = [&](Evaluate eval){
        if(next_field.size() <= beam_width){

            procon::NeoField field_buf = fields[eval.fields_index];
            ConnectedResult connect_result = PolygonConnector::connect(fields[eval.fields_index].getFrame()[eval.frame_index]
                    ,eval.is_inversed
                        ? fields[eval.fields_index].getElementaryInversePieces()[eval.piece_index]
                        : fields[eval.fields_index].getElementaryPieces()[eval.piece_index]
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

                //deplicacte flag
                bool flag = false;
                const std::string now_hash = hashField(field_buf);

                //check field is ok
                std::for_each(next_field.begin(),next_field.end(),[&](procon::NeoField f){
                    if(!flag){
                        if(now_hash == hashField(f)){
                            flag = true;
                        }
                    }
                });

                if(!flag){
                    if(!this->checkCanPrune(field_buf)){
                        next_field.push_back(field_buf);
                    }
                }
            });
            if(!flag){
                field_buf.evaluate_cache.push_back(eval);
                next_field.push_back(field_buf);
            }

            /*else{
                std::array<bool,50> is_placed = field_buf.getIsPlaced();
                is_placed[eval.piece_index] = true;
                field_buf.setIsPlaced(is_placed);

                field_buf.setPiece(std::get<1>(connect_result));

                debug_field.push_back(field_buf);
            }*/
        }
    };

    std::sort(evaluations.begin(),evaluations.end(),[](Evaluate l,Evaluate r){
        return l.score > r.score;
    });

    for(auto const& eval : evaluations){
        makeNextFieldFromEvaluate(eval);
    }
    fields.clear();
    std::copy(next_field.begin(),next_field.end(),std::back_inserter(fields));
#else
    //mulutithread mode
    int evaluate_counter = 0;

    auto makeNextFieldFromEvaluate = [&](){

        while(true){

            Evaluate eval;

            {
                std::lock_guard<decltype(mtx)> lock(mtx);
                if(evaluations.empty()){
                    return;
                }else if(next_field.size() >= beam_width){
                    return;
                }

                eval = evaluations[evaluations.size() - 1];
                evaluations.pop_back();

                ++evaluate_counter;
                logger->info("evaluate counter:" + std::to_string(evaluate_counter));
            }

            logger->info("evaluating");

            procon::NeoField field_buf = fields[eval.fields_index];
            ConnectedResult connect_result = PolygonConnector::connect(fields[eval.fields_index].getFrame()[eval.frame_index]
                    ,eval.is_inversed
                        ? fields[eval.fields_index].getElementaryInversePieces()[eval.piece_index]
                        : fields[eval.fields_index].getElementaryPieces()[eval.piece_index]
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

                //deplicacte flag
                bool flag = false;
                const std::string now_hash = hashField(field_buf);

                //check field is ok
                {
                    std::lock_guard<decltype(mtx)> lock(mtx);
                std::for_each(next_field.begin(),next_field.end(),[&](const procon::NeoField& f){
                    if(!flag){
                        if(now_hash == hashField(f)){
                            flag = true;
                        }
                    }
                });
                }

                if(!flag){
                    if(!this->checkCanPrune(field_buf)){
                        {
                            std::lock_guard<decltype(mtx)> lock(mtx);
                            next_field.push_back(field_buf);
                        }
                    }
                }
            }

            /*else{
                std::array<bool,50> is_placed = field_buf.getIsPlaced();
                is_placed[eval.piece_index] = true;
                field_buf.setIsPlaced(is_placed);

                field_buf.setPiece(std::get<1>(connect_result));

                debug_field.push_back(field_buf);
            }*/


        }
    };


    std::sort(evaluations.begin(),evaluations.end(),[](Evaluate l,Evaluate r){
        return l.score < r.score;
    });

    std::vector<std::thread> threads(cpu_num);
    for(auto& th : threads){
        th = std::thread(makeNextFieldFromEvaluate);
    }

    for(auto& th : threads){
        th.join();
    }

    logger->info("thread joined");

    fields.clear();
    std::copy(next_field.begin(),next_field.end(),std::back_inserter(fields));
#endif
}

bool BeamSearch::checkCanPrune(const procon::NeoField &field)
{
    clock_t a1,a2,c1,c2,d1,d2,e1,e2;

    //角について枝切りできるかできないか
    auto about_angle = [&field](){


        double hoge_frame_angle;
        double min_frame_angle = 360;
        for(auto const &frame : field.getFrame()){
            for(int angle = 0; angle < frame.getSize(); ++angle){
                hoge_frame_angle = frame.getSideAngle().at(angle);
                if(hoge_frame_angle < min_frame_angle) min_frame_angle = hoge_frame_angle;
            }
        }
        std::cout << "枠の最小角は " << min_frame_angle << std::endl;

        double hoge_piece_angle;
        double min_piece_angle = 360;
        for(auto const &piece : field.getElementaryPieces()){
            for(int pieceangle = 0; pieceangle < piece.getSize(); ++pieceangle){
                hoge_piece_angle = piece.getSideAngle().at(pieceangle);
                if(hoge_piece_angle < min_piece_angle) min_piece_angle = hoge_piece_angle;
            }
        }
        std::cout << "ピースの最小角は " <<min_piece_angle << std::endl;

        if(min_frame_angle < min_piece_angle){
            return true;
        }else{
            return false;
        }
    };
    //辺について枝きりできるかできないか
    /*auto about_side = [&field](){
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
    */
    //対角線で枝きりできるか否か
    auto about_distance = [&field](){
        struct tool{                //関数内関数のための構造体
            static int Distance(point_i point_a,point_i point_b){ //2点を引数にその間の距離の二乗を返す
                int a = point_a.x();
                int b = point_a.y();
                int c = point_b.x();
                int d = point_b.y();
                int distance = (a - c)*(a - c) + (b - d)* (b - d);
                return distance;
            }
            static int polygondistance(polygon_i polygon){   //ポリゴンを引数にとってその中で一番長い辺or対角線の二乗をかえす。
                int distance = 0,instance;
                for(point_i point_a : polygon.outer()){
                    for(point_i point_b : polygon.outer()){
                        instance = Distance(point_a,point_b);
                        if(instance > distance){
                            distance = instance;
                        }
                    }
                }
                return distance;
            }
        };
        int most_frame = 0,most_piece = 0,frame_instance,piece_instance;
        for(auto const neoframe :field.getFrame()){
             polygon_i frame =neoframe.procon::NeoExpandedPolygon::getPolygon();
             frame_instance = tool::polygondistance(frame);
             if(most_frame < frame_instance){
                 most_frame = frame_instance;
             }
        }
        for(auto const neopiece : field.getElementaryPieces()){
            polygon_i piece = neopiece.procon::NeoExpandedPolygon::getPolygon();
            piece_instance = tool::polygondistance(piece);
            if(most_piece < piece_instance){
                most_piece = piece_instance;
            }
        }
        if(most_piece > most_frame){
            std::cout <<"対角線で枝きり"<<std::endl;
            return true;
        }
        std::cout <<"対角線では無理だった"<<std::endl;
        return false;
    };


    auto framesize_single = [&field](procon::NeoExpandedPolygon frame){ //一つのフレームとピースとの面積が合致するかを出す関数    時間に対して弾けるパターンが少なすぎるので改善しましょう
        /*
        std::vector<double> area_vec;
        const double frame_area = bg::area(frame.getPolygon());
        std::cout << field.getElementaryPieces().size() << field.getFrame().size() << std::endl;
        for(auto piece : field.getElementaryPieces()){
            double area = bg::area(piece.getPolygon());
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
                    double add_cou = area + add_vec.at(vec_count);
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
        return true;*/


        std::vector<double> piecearea_vec;
        for(auto piece : field.getElementaryPieces()){
            piecearea_vec.push_back(bg::area(piece.getPolygon()));
            std::cout << bg::area(piece.getPolygon());
        }

        std::cout << field.getElementaryPieces().size() << std::endl;
        const int piece_cou = field.getElementaryPieces().size();
        for(int cou=0;cou < std::pow(2,piece_cou );++cou){
            double add_number = 0;//ここの数値に対応する値を加算していく
            for(int digit=0;digit<piece_cou;++digit){
                int count = std::pow(2 , digit);//pieceの合計が3つなら4,2,1みたいな感じのが出る(100,010,001みたいになる)
                if(count & cou){//100,101みたいな感じなら100が返ってくるからn桁目が0か1かが分かる
                    add_number += piecearea_vec.at(digit);
                }
            }
            std::cout << add_number << "  " << bg::area(frame.getPolygon()) << std::endl;
           // std::cout << std::pow(2,piece_cou) << " " << cou << std::endl;
            if(add_number == bg::area(frame.getPolygon()))return false;
        }


        return true;
    };

    //複数のFrameがあるときにピースと面積が合致するか
    auto about_framesize = [&field,&framesize_single](){
        const int frame_size_max = 1000;//これより大きい面積のframeは判定しない(処理に時間がかかるため)
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



    //複数のframeがある時にその内角を満たす角の組み合わせが存在するか調べる
    auto about_frameangle = [&field](){
        const int frame_angle_max = 180;
        auto calculation_rad = [](double a){
            //ラジアン(radian)から角度(degree)に変換
            return a * 180.0 / 3.141592653589793;
        };

        std::vector<double> frameangle_vec;
        std::vector<double> add_vec;

        auto erase_frame = [&frameangle_vec,&add_vec](double angle){
                for(int count=0;count<frameangle_vec.size();++count){
                    double frame_angle = frameangle_vec.at(count);
                    if(frame_angle < angle + 0.01 && frame_angle > angle - 0.01){
                        frameangle_vec.erase(frameangle_vec.begin() + count,frameangle_vec.end());
                        std::cout << "frameangle_vec.size : " << frameangle_vec.size() << std::endl;
                        if(frameangle_vec.size())return true;
                        break;
                    }
                }
                return false;
        };

        auto erase_frame_vec = [&frameangle_vec,&add_vec](){
            bool flag;
            while(!flag){
                flag=true;
                for(int count=0;count<frameangle_vec.size();++count){
                    double frame_angle = frameangle_vec.at(count);
                    for(auto angle : add_vec){
//                        std::cout << "frame_angle : " << frame_angle << "  " << angle << std::endl;
                        if(frame_angle < angle + 0.01 && frame_angle > angle - 0.01){
                            std::cout << "かぶってます" << frame_angle << "  " << angle << std::endl;
                            frameangle_vec.erase(frameangle_vec.begin() + count,frameangle_vec.end());
                            std::cout << "frameangle_vec.size : " << frameangle_vec.size() << std::endl;
                            flag=false;
                            break;
                        }else if(frame_angle < angle + 0.01)break;
                    }
                }
            }
            if(frameangle_vec.size()==0)return true;
            return false;
        };


        std::vector<double> pieceangle_vec;
        for(auto frame : field.getFrame()){

            for(auto side_angle_ : frame.getSideAngle()){
                double side_angle = calculation_rad(side_angle_);
                std::cout << side_angle << std::endl;
                if(side_angle < frame_angle_max)frameangle_vec.push_back(side_angle);
            }
        }
        std::sort(frameangle_vec.begin(),frameangle_vec.end());
        frameangle_vec.erase(std::unique(frameangle_vec.begin(),frameangle_vec.end()),frameangle_vec.end());
        for(auto piece : field.getElementaryPieces()){
            for(auto side_angle_ : piece.getSideAngle()){
                double side_angle = calculation_rad(side_angle_);
                pieceangle_vec.push_back(side_angle);
            }
        }

        std::cout << "frame_vec一覧表示 : ";
        for(auto angle : frameangle_vec){
            std::cout << angle << " ";
        }
        std::cout << std::endl;

        std::sort(pieceangle_vec.begin(),pieceangle_vec.end());
        for(int count=0;count<pieceangle_vec.size();++count){
           if(pieceangle_vec.at(count) >= frame_angle_max){
               pieceangle_vec.erase(pieceangle_vec.begin() + count,pieceangle_vec.end());
               break;
           }
        }
        pieceangle_vec.erase(std::unique(pieceangle_vec.begin(),pieceangle_vec.end()),pieceangle_vec.end());

        add_vec = pieceangle_vec;
        if(erase_frame_vec())return false;

        std::cout << "frame_vec一覧表示 : ";
        for(auto angle : frameangle_vec){
            std::cout << angle << " ";
        }
        std::cout << std::endl;

        std::cout << "piece_vec一覧表示 : ";
        for(auto angle : pieceangle_vec){
            std::cout << angle << " ";
        }
        std::cout << std::endl;


        bool flag= false;
        for(unsigned int count=0;count<pieceangle_vec.size();++count){
            for(int add_count=0;add_count<add_vec.size();++add_count){
                bool check=false;
                for(auto piece_angle : pieceangle_vec){
                    double add_number = add_vec.at(add_count) + piece_angle;
                    if(add_number < frame_angle_max){
                        add_vec.push_back(add_number);
                        if(erase_frame(add_number))return false;
                        check=true;
                    }
                    else break;
                }
                if(!check){//ここまで行けばループ脱出
                    flag=true;
                    break;
                }
                std::sort(add_vec.begin(),add_vec.end());
                add_vec.erase(std::unique(add_vec.begin(),add_vec.end()),add_vec.end());
                //if(erase_frame_vec())return false;
            }
            if(flag)break;
        }

        std::cout << "frame_vec一覧表示 : ";
        for(auto angle : frameangle_vec){
            std::cout << angle << " ";
        }
        std::cout << std::endl;

        std::cout << "piece_vec一覧表示 : ";
        for(auto angle : pieceangle_vec){
            std::cout << angle << " ";
        }
        std::cout << std::endl;

        std::cout << "add_vec一覧表示 : ";
        for(auto angle : add_vec){
            std::cout << angle << " ";
        }
        std::cout << std::endl;

        if(erase_frame_vec())return false;

        return true;
    };
    //枝切り関数の時間計測部分
/*
    a1 = clock();
    bool a = about_angle();
    a2 = clock();
    c1 = clock();
    bool c = about_framesize();
    c2 = clock();
    d1 = clock();
    bool d = about_distance();
    d2 = clock();
    e1 = clock();
    bool e = about_frameangle();//ここまで進んだ後に何も出力されず終了　多分これ動いてないぞ
    e2 = clock();
    std::cout << "aの実行時間は " << a2 - a1 <<std::endl;
    std::cout << "cの実行時間は " << c2 - c1 <<std::endl;
    std::cout << "dの実行時間は " << d2 - d1 <<std::endl;
    std::cout << "eの実行時間は " << e2 - e1 <<std::endl;
    return a || c || d || e;
    */
    //枝切り関数の時間計測終わり使わないときはコメントアウトで
    bool a = about_distance();
    if(a)return a;
    bool b = about_angle();
    if(b)return b;
//    bool c = about_framesize();
//    if(c)return c;
//    bool d = about_frameangle();
//    if(d)return d;
    return false;
    //OKならfalseを返す
}

int BeamSearch::checkOddField(const procon::NeoField &field, const Connect &connector, const int field_frame_index, const int field_piece_index) //field_frame_indexはConnectクラスのindexで参照すべきframeのindex
{
    //mag = magnification（倍率）
    int odd_score = 0; //ゲテモノポイント
    const int odd_connect_length = 3; //これ以下の長さだとゲテモノ（単位はグリット）
    const int check_connect_length_mag = 10; //１倍のときのゲテモノポイント加算値はodd_connect_length以下の整数になる

    std::vector<procon::NeoExpandedPolygon> frames = field.getFrame();

    auto check_connect_length = [&]() //結合した辺のゲテモノ度
    {
        procon::NeoExpandedPolygon frame = frames.at(static_cast<unsigned int>(field_frame_index));
        std::vector<double> frame_lengthes = frame.getSideLength();
        double frame_length = frame_lengthes.at(static_cast<unsigned int>(connector.frame_side_index));

        auto checklength = [](double length)
        {
            double length_int;
            double length_double = modf(length, &length_int);
            return static_cast<int>((length_double > 0.0) ? length_int + 1.0 : length_int);
        };

        int frame_length_up_int = checklength(frame_length);
        if(frame_length_up_int <= odd_connect_length) odd_score += (odd_connect_length + 1 - frame_length_up_int) * check_connect_length_mag;
        else {
            procon::NeoExpandedPolygon piece = field.getPiece(field_piece_index);
            std::vector<double> piece_lengthes = piece.getSideLength();
            double piece_length = piece_lengthes.at(static_cast<unsigned int>(connector.polygon_side_index));
            int piece_length_up_int = checklength(piece_length);
            if(piece_length_up_int <= odd_connect_length) odd_score += (odd_connect_length + 1 - piece_length_up_int) * check_connect_length_mag;
        }
    };

    if(odd_connect_length > 0 && check_connect_length_mag > 0) check_connect_length();

    return odd_score;
}

void BeamSearch::evaluateNextState(std::vector<procon::NeoField> & fields,std::vector<Evaluate> & evaluations)
{
    //frameがstd::vector<NeoExPolygon>なのでそれぞれに対して、評価関数を回す
    auto evaluateWrapper = [&](procon::NeoField const& field,int const& piece_index,int const& fields_index){
        int frame_index = 0;
        for(const auto& f : field.getFrame()){
            //inverseしていない方のpiece評価
            std::vector<std::pair<double,Connect>> evaluate = Evaluation::evaluation(f,field.getElementaryPieces()[piece_index],1.0,1.0,true);
            //inverseしている方のpiece評価
            std::vector<std::pair<double,Connect>> evaluate_inversed = Evaluation::evaluation(f,field.getElementaryInversePieces()[piece_index],1.0,1.0,true);

            //一時保存用の変数
//            TODO: いい感じにここをパフォーマンスよくする
            Evaluate ev_buf;
            for(const auto& e : evaluate){
                if(e.first){
                ev_buf.score = e.first;
                ev_buf.connection = e.second;
                ev_buf.fields_index = fields_index;
                ev_buf.frame_index = frame_index;
                ev_buf.piece_index = piece_index;
                ev_buf.is_inversed = false;
                evaluations.push_back(ev_buf);

                }
            }
            for(const auto& e : evaluate_inversed){
                if(e.first){
                    ev_buf.score = e.first;
                ev_buf.connection = e.second;
                ev_buf.fields_index = fields_index;
                ev_buf.frame_index = frame_index;
                ev_buf.piece_index = piece_index;
                ev_buf.is_inversed = true;
                evaluations.push_back(ev_buf);

                }
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
    int field_index = 0;
    for(auto const& f : fields){
        evaluateNextState(f,field_index);
        ++field_index;
    }

#endif
}

void BeamSearch::init()
{
#ifdef DEBUG_MODE
//    logger->info("debug mode");
#else
    logger->info("efficient mode");
#endif
}
void BeamSearch::run(procon::NeoField field)
{
    logger->info("beamsearch run");
    dock->addAnswer(field);
//    logger->info("beamsearch run");

    std::vector<procon::NeoField> state;

    //フレームの同じ傾きの頂点を除去
    auto delete_deplicate_point = [](procon::NeoField & field){
        int frame_index = 0;
        for(auto & frame : field.getFrame()){
            std::vector<int> parallel_dot;
            int count = 0;
            for(auto const& angle : frame.getSideAngle()){
                if(angle == M_PI){
                    parallel_dot.insert(parallel_dot.begin(),count);
                }
                ++count;
            }
            if(parallel_dot.size()){
                std::vector<point_i> points;
                std::vector<procon::NeoExpandedPolygon> frames;

                std::copy(field.getFrame().begin(),field.getFrame().end(),std::back_inserter(frames));

                frames.erase(frames.begin() + frame_index);

                std::copy(frame.getPolygon().outer().begin(),
                          frame.getPolygon().outer().end(),
                          std::back_inserter(points)
                );

                for(auto const& c : parallel_dot){
                    points.erase(points.begin() + c);
                }

                procon::NeoExpandedPolygon f;
                polygon_i p;
                for(auto const& point : points){
                    p.outer().push_back(point);
                }
                f.resetPolygonForce(p);

                frames.insert(frames.begin() + frame_index,f);
                field.setFrame(frames);
            }
            ++frame_index;
        }
    };


#ifdef DEBUG_MODE
    delete_deplicate_point(field);
#endif

    state.push_back(field);

//    ev.resize(2000000);
    for (int piece_num = 0; piece_num < static_cast<int>(field.getElementaryPieces().size()); ++piece_num) {
        std::vector<Evaluate> ev;

        logger->info("next step start");

        evaluateNextState(state,ev);

        logger->info("evaluating field process has finished");

        makeNextState(state,ev);

        logger->info("making field process has finished");

        std::cout << "now" << (piece_num + 1) << "/" << field.getElementaryPieces().size() << std::endl;
        std::cout << "evaluated state size:" << ev.size() << std::endl;
        std::cout << "field size:" << state.size() << std::endl;

        //vectorのメモリ解放って頭悪くね？
        std::vector<Evaluate>().swap(ev);

        for(auto& f : state){
            delete_deplicate_point(f);
        }

        bool flag = false;
        for(auto const& _field : state){
            dock->addAnswer(_field);
            if(!flag){
                submitAnswer(_field);
                flag = true;
            }
        }

//        if(piece_num == 4){
//            break;
//        }
    }

    //    neo = std::make_shared<NeoAnswerDock>();
    //    neo->show();
    //    for(const auto& f : this->debug_field){
    //        neo->addAnswer(f);
    //    }
    //    test();
}

