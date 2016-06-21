#ifndef POLYGONEXPANSION_H
#define POLYGONEXPANSION_H
#include <iostream>
#include <boost/geometry.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <vector>
#include <cmath>
namespace bg = boost::geometry;
typedef bg::model::d2::point_xy<double> point_t;
typedef bg::model::polygon<point_t> polygon_t;

class PolygonExpansion
{
    polygon_t polygon;
    int n; //n角形
    std::vector<double> sideLength;
    std::vector<double> sideAngle;
    void calcSideLength();
    void calcN();
    void calcSideAngle();
public:
    PolygonExpansion();
    polygon_t getPolygon();
    const std::vector<double> getSideLength();
    const std::vector<double> getSideAngle();
    void setPolygon(const polygon_t &pol);
};

#endif // POLYGONEXPANSION_H
