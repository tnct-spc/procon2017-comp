#ifndef POLYGONCONNECTOR_H
#define POLYGONCONNECTOR_H

#include <array>
#include <vector>
#include "field.h"
#include "Algorithm/algorithmwrapper.h"
#include "Utils/fit.h"
#include <cmath>

using Ring = std::vector<point_t>;

class PolygonConnector
{
public:
    static procon::ExpandedPolygon joinPolygon(procon::ExpandedPolygon Polygon1, procon::ExpandedPolygon Polygon2, std::array<Fit,2> join_data);
private:
    PolygonConnector();
    static Ring popRingByPolygon(procon::ExpandedPolygon& polygon, int inner_position = -1);
    static void pushRingToPolygon(Ring& ring, procon::ExpandedPolygon& polygon, int inner_position = -1);
};

#endif // POLYGONCONNECTOR_H
