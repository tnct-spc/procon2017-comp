#include <QCoreApplication>
#include <boost/geometry/geometries/multi_polygon.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/algorithms/area.hpp>
#include <boost/geometry.hpp>
#include <boost/assign/list_of.hpp>
#include <cmath>
#include <vector>
#include <iostream>
#include "field.h"
#include "testpolygon.h"
#include "polygonio.h"

TestPolygon::TestPolygon()
{

}

bool TestPolygon::run(){
    /*polygon*/
    std::cout << "Hello,polygon!" << std::endl;
    polygon_t b,c,d;
    bg::exterior_ring(b) = boost::assign::list_of<point_t>(0, 0)(0, 30.12)(30.14,30.17)(15.92,15.42)(30.11, 0)(0, 0);
    bg::exterior_ring(c) = boost::assign::list_of<point_t>(0, 0)(0,1)(1,0)(0,0);
    bg::model::linestring<point_t> seg1 = boost::assign::list_of<point_t>(0,0)(1,1);
    bg::model::linestring<point_t> seg2 = boost::assign::list_of<point_t>(0,0)(0,1);
    std::cout << bg::intersects(seg1,seg2) << std::endl;
    procon::ExpandedPolygon hoge;
    hoge.setPolygon(b);
    std::cout << bg::area(b) << std::endl;
    auto angle= hoge.getSideAngle();
    for(auto a : angle){
        std::cout << a << std::endl;
    }
    return true;
}
