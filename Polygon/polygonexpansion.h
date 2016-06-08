#ifndef POLYGONEXPANSION_H
#define POLYGONEXPANSION_H
#include <iostream>
#include <boost/geometry.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <vector>
namespace bg = boost::geometry;
typedef bg::model::d2::point_xy<double> point_t;
typedef bg::model::polygon<point_t> polygon_t;

class PolygonExpansion
{
    polygon_t polygon;
    std::vector<double> sideLength;
    std::vector<double> sideAngle;
public:
    PolygonExpansion();
};

#endif // POLYGONEXPANSION_H
