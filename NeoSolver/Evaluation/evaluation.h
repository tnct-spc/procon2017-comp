#ifndef EVALUATION_H
#define EVALUATION_H
#include "neoexpandedpolygon.h"
#include "Utils/polygonconnector.h"

class Evaluation
{
public:
    Evaluation();
    static int calculation_nep (const procon::NeoExpandedPolygon &nep , int index , int cal);
    std::vector<std::pair<double , Connect>> evaluation(procon::NeoExpandedPolygon const& frame , procon::NeoExpandedPolygon const& polygon , double angle_weight , double side_weight);
};

#endif // EVALUATION_H
