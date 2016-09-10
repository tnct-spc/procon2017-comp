#ifndef POLYGONCONNECTOR_H
#define POLYGONCONNECTOR_H

#include <array>
#include "field.h"
#include "Algorithm/algorithmwrapper.h"
#include "Utils/fit.h"
#include <cmath>

class PolygonConnector
{
public:
    static procon::ExpandedPolygon joinPolygon(procon::ExpandedPolygon Polygon1, procon::ExpandedPolygon Polygon2, std::array<Fit,2> join_data);
private:
    PolygonConnector();
    static procon::ExpandedPolygon newPolygonDate(procon::ExpandedPolygon polygon);
};

#endif // POLYGONCONNECTOR_H
