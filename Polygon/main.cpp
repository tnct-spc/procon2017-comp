#include <QCoreApplication>
#include <boost/geometry/geometries/multi_polygon.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/algorithms/area.hpp>
#include <boost/geometry.hpp>
#include <boost/assign/list_of.hpp>
#include <cmath>
#include <vector>
#include <iostream>
namespace bg = boost::geometry;
typedef bg::model::d2::point_xy<double> point_xy_t;
typedef bg::model::polygon<point_xy_t> polygon_t;
typedef bg::model::multi_polygon<polygon_t> multi_polygon_t;

int main(int argc,char *argv[])
{
    /*polygon*/
    QCoreApplication a(argc, argv);
    std::cout << "Hello,polygon!" << std::endl;
    polygon_t Mr_Kato;
    multi_polygon_t mizuki_chan;
    mizuki_chan.resize(50);
    bg::exterior_ring(Mr_Kato) = boost::assign::list_of<point_xy_t> (0,0)(0,3)(3,3)(0,0);
    bg::exterior_ring(mizuki_chan[0]) = boost::assign::list_of<point_xy_t> (0,0)(0,3)(3,3)(0,0);
    const double polygon_area = bg::area(mizuki_chan[0]); //面積
    const point_xy_t polygon_center = bg::return_centroid<point_xy_t>(mizuki_chan[0]); //中心点
    std::cout << polygon_area << std::endl;
    std::cout << bg::dsv(polygon_center) << std::endl;
    return a.exec();
}
