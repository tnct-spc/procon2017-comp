#ifndef NEOEXPANDEDPOLYGON_H
#define NEOEXPANDEDPOLYGON_H

#include "fit.h"
#include <iostream>
#include <vector>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>


namespace bg = boost::geometry;
typedef bg::model::d2::point_xy<int> point_i;
typedef bg::model::polygon<point_i> polygon_i;

namespace procon {

class NeoExpandedPolygon
{
    //constractor
    NeoExpandedPolygon();

private:
    polygon_i polygon;

public:
    //getter
    polygon_i getPolygon();

    //setter
    void setPolygon(polygon_i polygon);

};

}

#endif // NEOEXPANDEDPOLYGON_H
