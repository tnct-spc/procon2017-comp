#ifndef POLYGON_H
#define POLYGON_H
#include <iostream>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry.hpp>
#include <boost/assign/list_of.hpp>

namespace bg = boost::geometry;
typedef bg::model::d2::point_xy<double> point_xy_t;
typedef bg::model::polygon<point_xy_t> polygon_t;
typedef bg::model::multi_polygon<polygon_t> multi_polygon_t;

class Polygon
{
    multi_polygon_t mizuki_chan;
public:
    Polygon();
    void setPolygon(const polygon_t &tmp);
    void setPointXy(const std::vector<point_xy_t> &tmp);
};
#endif // POLYGON_H
