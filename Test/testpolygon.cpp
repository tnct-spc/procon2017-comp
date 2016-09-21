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
    auto printVector = [](std::vector<double> vec){
        for (auto v : vec){
            std::cout << v << ',';
        }
        std::cout << std::endl;
    };

    std::cout << "Hello,polygon!" << std::endl;
    polygon_t b,c,d;
    bg::exterior_ring(b) = boost::assign::list_of<point_t>(0, 0)(0, 30.14)(30.14,30.14)(15.92,15.42)(30.11, 0)(0, 0);
    bg::exterior_ring(c) = boost::assign::list_of<point_t>(0, 0)(0,30.14)(14.99,30.14)(15.03,15.01)(15.001,30.02)(0,30.1)(0,0);
    bg::model::linestring<point_t> seg1 = boost::assign::list_of<point_t>(0,0)(1,1);
    bg::model::linestring<point_t> seg2 = boost::assign::list_of<point_t>(0,0)(0,1);
    std::cout << bg::intersects(seg1,seg2) << std::endl;
    procon::ExpandedPolygon hoge;

    hoge.setPolygon(c);
    std::cout << bg::dsv(hoge.getPolygon()) << std::endl;
    /*
    hoge.rotatePolygon(90);
    std::cout << bg::dsv(hoge.getPolygon()) << std::endl;
    printVector(hoge.getSideSlope());

    hoge.setPolygonAngle(270);
    std::cout << "ang" << bg::area(hoge.getPolygon()) << std::endl;
    std::cout << bg::dsv(hoge.getPolygon()) << std::endl;
    printVector(hoge.getSideSlope());
    */
    hoge.inversePolygon();
    std::cout << "inv" << bg::area(hoge.getPolygon()) << std::endl;
    std::cout << bg::dsv(hoge.getPolygon()) << std::endl;

    std::cout << "end" << std::endl;
    return true;
}
