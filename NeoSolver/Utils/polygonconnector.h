#ifndef POLYGONCONNECTOR_H
#define POLYGONCONNECTOR_H

#include "precompile.h"
#include "neoexpandedpolygon.h"

namespace bg = boost::geometry;
typedef std::complex<double> complex_d;

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
    std::tuple<std::vector<procon::NeoExpandedPolygon>, procon::NeoExpandedPolygon, bool> connect(procon::NeoExpandedPolygon frame, procon::NeoExpandedPolygon piece, Connect connecter);
    std::pair<polygon_i, bool> rotate(polygon_i polygon_in, complex_d after_angle, int first_index, int second_index);
};

#endif // POLYGONCONNECTOR_H
