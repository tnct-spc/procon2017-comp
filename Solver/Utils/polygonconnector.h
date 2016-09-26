#ifndef POLYGONCONNECTOR_H
#define POLYGONCONNECTOR_H

#include "expandedpolygon.h"
#include "fit.h"

using Ring = std::vector<point_t>;

class PolygonConnector
{
public:
    static bool joinPolygon(procon::ExpandedPolygon Polygon1, procon::ExpandedPolygon Polygon2, procon::ExpandedPolygon& new_polygon, std::array<Fit,2> join_data);
private:
    static int increment(int num, int size){return (num + 1) % size;}
    static int decrement(int num, int size){return ((num - 1) % size + size) % size;}

    PolygonConnector();
    static Ring popRingByPolygon(procon::ExpandedPolygon& polygon, int inner_position = -1);
    static polygon_t pushRingToPolygonT(Ring& ring, procon::ExpandedPolygon const& polygon, int inner_position = -1);
    static bool hasConflict(Ring ring1, Ring ring2, Fit fit1, Fit fit2);
};

#endif // POLYGONCONNECTOR_H
