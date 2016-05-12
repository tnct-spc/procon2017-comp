#include <QCoreApplication>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp> 

#include <boost/geometry/index/rtree.hpp>

#include <cmath>
#include <vector>
#include <iostream>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

int main(int argc,char *argv[])
{
    /*polygon*/
    QCoreApplication a(argc, argv);
    typedef boost::geometry::model::d2::point_xy<double> point_xy_t;
    //double型実体のbgm::d2::point_xyの型をpoint_xy_tと書ける
    typedef boost::geometry::model::polygon<std::initializer_list<point_xy_t>> polygon_t;
    //std::vector<point_xy_t>型実体のbgm::polygonの型をpolygon_tと書ける
    point_xy_t piyo(2,1);
    polygon_t huga();
    return a.exec();
}
