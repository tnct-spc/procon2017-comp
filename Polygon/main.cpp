#include <QCoreApplication>
#include <boost/geometry/geometries/multi_polygon.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/algorithms/area.hpp>
#include <boost/geometry.hpp>
#include <cmath>
#include <vector>
#include <iostream>
namespace bg = boost::geometry;
typedef boost::geometry::model::d2::point_xy<double> point_xy_t;
typedef boost::geometry::model::polygon<point_xy_t> polygon_t;
typedef boost::geometry::model::multi_polygon<polygon_t> multi_polygon_t;

int main(int argc,char *argv[])
{
    /*polygon*/
    QCoreApplication a(argc, argv);
    std::cout << "Hello,polygon!" << std::endl;
    polygon_t hoge;
    std::cout << boost::geometry::area(huga) << std::endl;
    multi_polygon_t huga;
    huga.resize(50); //最大50個
    huga[0] = hoge;
    return a.exec();
}
