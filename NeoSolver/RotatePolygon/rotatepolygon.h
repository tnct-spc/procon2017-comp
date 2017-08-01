#ifndef ROTATEPOLYGON_H
#define ROTATEPOLYGON_H

#include "precompile.h"

namespace bg = boost::geometry;
typedef bg::model::d2::point_xy<int> point_i;
typedef bg::model::polygon<point_i> polygon_i;
typedef std::complex<double> complex_d;

std::vector<polygon_i> rotate_polygon(polygon_i polygon_in);

#endif // ROTATEPOLYGON_H
