#ifndef POLYGONCONNECTOR_H
#define POLYGONCONNECTOR_H

#include "precompile.h"
#include "neoexpandedpolygon.h"
#include "complexangle.h"

namespace bg = boost::geometry;
using point_t = bg::model::d2::point_xy<double>;
using polygon_t = bg::model::polygon<point_t,true,true,std::vector,std::vector,std::allocator,std::allocator>;
typedef std::complex<int> complex_i;

class Connect
{
public:
    //接するべき辺の若い方のpointのindex
    int frame_side_index = -1;
    int polygon_side_index = -1;

    //触れてるべき頂点のindex
    int frame_point_index = -1;
    int polygon_point_index = -1;

};

class PolygonConnector
{
public:
    PolygonConnector();
    static std::tuple<std::vector<procon::NeoExpandedPolygon>, procon::NeoExpandedPolygon, bool> connect(procon::NeoExpandedPolygon frame, procon::NeoExpandedPolygon piece, Connect connecter);
    static std::pair<polygon_i, bool> rotate(polygon_i polygon_in, ComplexAngle after_angle, unsigned int first_index, unsigned int second_index);
};

#endif // POLYGONCONNECTOR_H
