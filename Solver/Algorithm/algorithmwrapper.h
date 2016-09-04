 #ifndef ALGORITHMWRAPPER_H
#define ALGORITHMWRAPPER_H

#include "field.h"

class AlgorithmWrapper
{
public:
    AlgorithmWrapper();
    procon::ExpandedPolygon newPolygonDate(procon::ExpandedPolygon polygon);
    procon::ExpandedPolygon joinPolygon(procon::ExpandedPolygon Polygon1,int Polygon1_start_pos,int Polygon1_end_pos, procon::ExpandedPolygon Polygon2,int Polygon2_start_pos,int Polygon2_end_pos);
    virtual procon::Field run(procon::Field field);
};

#endif // ALGORITHMWRAPPER_H
