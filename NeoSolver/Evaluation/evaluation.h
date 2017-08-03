#ifndef EVALUATION_H
#define EVALUATION_H
#include "neoexpandedpolygon.h"
#include "Utils/polygonconnector.h"

class Evaluation
{
public:
    Evaluation();
    std::vector<std::pair<int , Connect>> evaluation(procon::NeoExpandedPolygon const& frame , procon::NeoExpandedPolygon const& polygon);
};

#endif // EVALUATION_H
