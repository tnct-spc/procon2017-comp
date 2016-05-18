#ifndef POLYGON_H
#define POLYGON_H
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <vector>
typedef boost::geometry::model::d2::point_xy<double> point_xy_double;
typedef boost::geometry::model::polygon<point_xy_double> polygon_t;
class polygon
{
    std::vector<polygon_t> Mr_kato;
public:
    polygon();
    void push_back(const polygon_t &tmp);
};

#endif // POLYGON_H
