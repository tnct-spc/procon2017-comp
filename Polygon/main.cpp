#include <QCoreApplication>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp> 
#include <cmath>
#include <vector>
#include <iostream>

int main(int argc,char *argv[])
{
    /*polygon*/
    QCoreApplication a(argc, argv);
    typedef boost::geometry::model::d2::point_xy<double> point_xy_t;
    //double型実体のbgm::d2::point_xyの型をpoint_xy_tと書ける
    typedef boost::geometry::model::polygon<point_xy_t> polygon_t;
    //point_xy_t型実体のbgm::polygonの型をpolygon_tと書ける
    point_xy_t x(0,1),y(0,0),z(1,0);
    polygon_t huga={{x},{y},{z}};
    return a.exec();
}
