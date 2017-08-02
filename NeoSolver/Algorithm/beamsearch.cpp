#include "beamsearch.h"
#include "expandedpolygon.h"
#include "neopolygonviewer.h"
#include "polygonviewer.h"

#define DEBUG_MODE

void connect_polygon(polygon_i& frame, polygon_i& connecting_polygon,Connect connection)
{
    NeoPolygonViewer::getInstance().displayPolygon(frame,"hogehoge",false);
    NeoPolygonViewer::getInstance().displayPolygon(connecting_polygon,"hogehoge",false);

	//まずくっつくべきpolygon点を原点に持ってきて回転の中心にする
    boost::geometry::strategy::transform::translate_transformer<int,2,2> transform_connect_point_to_origin(
    	-connecting_polygon.outer()[connection.polygon_point_num].x(),
        -connecting_polygon.outer()[connection.polygon_point_num].y()
    );
    polygon_i result;
    boost::geometry::transform(connecting_polygon,result,transform_connect_point_to_origin);
    
    std::pair<int,int> piece_vec = std::make_pair(
    	connecting_polygon.outer()[connection.polygon_side_num + 1].x() - connecting_polygon.outer()[connection.polygon_side_num].x(),
        connecting_polygon.outer()[connection.polygon_side_num + 1].y() - connecting_polygon.outer()[connection.polygon_side_num].y()
    );
    std::pair<int,int> frame_vec = std::make_pair(
        frame.outer()[connection.frame_side_num + 1].x() - frame.outer()[connection.frame_side_num].x(),
        frame.outer()[connection.frame_side_num + 1].y() - frame.outer()[connection.frame_side_num].y()
    );
    
    double piece_angle = std::atan2(piece_vec.first,piece_vec.second);
    double frame_angle = std::atan2(frame_vec.first,frame_vec.second);
   
    polygon_i result2;
    polygon_t result2_t;
    boost::geometry::strategy::transform::rotate_transformer<bg::radian,int,2,2> rotate_1(-(piece_angle - frame_angle));
    boost::geometry::strategy::transform::rotate_transformer<bg::radian,double,2,2> rotate_2(-(piece_angle - frame_angle));
    std::cout << "hogehoge" << -(piece_angle - frame_angle);
    std::cout << "hogehoge" << boost::geometry::transform(result,result2,rotate_1) << std::endl;
    std::cout << "hogehoge" << boost::geometry::transform(result,result2_t,rotate_2) << std::endl;

    procon::ExpandedPolygon pol;
    pol.resetPolygonForce(result2_t);
    PolygonViewer::getInstance().pushPolygon(pol,"kkk");
    NeoPolygonViewer::getInstance().displayPolygon(result2,"jjsg",false);
    
    boost::geometry::strategy::transform::translate_transformer<int,2,2> transform_to_connect_point(
    	frame.outer()[connection.frame_point_num].x(),
        frame.outer()[connection.frame_point_num].y()
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

    connect_polygon(sample1,sample2,con);
}

BeamSearch::BeamSearch()
{
    logger = spdlog::get("beamsearch");
}

void BeamSearch::init()
{
#ifdef DEBUG_MODE
    logger->info("debug mode");
#else
    logger->info("efficient mode");
#endif
}

void BeamSearch::run(procon::NeoField field)
{
    test();
}
