#include "beamsearch.h"
#include "expandedpolygon.h"
#include "neopolygonviewer.h"
#include "polygonviewer.h"
#include "Utils/polygonconnector.h"
#include "Evaluation/evaluation.h"
#include "parallel.h"

#include <thread>
#include <mutex>
#include <condition_variable>

#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <tbb/task_scheduler_init.h>
#include <tbb/tick_count.h>

//もしデバックモードにしたければ下をコメントアウト
#define DEBUG_MODE

//std::condition_variable cond;

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
        }else{
            std::array<bool,50> is_placed = field_buf.getIsPlaced();
            is_placed[eval.piece_index] = true;
            field_buf.setIsPlaced(is_placed);

            field_buf.setPiece(std::get<1>(connect_result));

            debug_field.push_back(field_buf);
        }
    };

#ifdef DEBUG_MODE
    std::sort(evaluations.begin(),evaluations.end(),[](Evaluate l,Evaluate r){
        return l.score > r.score;
    });

    for(auto const& eval : evaluations){
        makeNextFieldFromEvaluate(eval);

        //ビーム幅を超えたらああああ終わりです。
        if(next_field.size() == this->beam_width){
            break;
        }
    }
    fields.clear();
    std::copy(next_field.begin(),next_field.end(),std::back_inserter(fields));
#elif
    std::cout << "hogehoge"
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
    }

//    neo = std::make_shared<NeoAnswerDock>();
//    neo->show();
//    for(const auto& f : this->debug_field){
//        neo->addAnswer(f);
//    }
//    test();
}

std::string BeamSearch::hashField(procon::NeoField field){
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
