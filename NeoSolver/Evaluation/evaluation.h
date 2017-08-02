#ifndef EVALUATION_H
#define EVALUATION_H
#include "neoexpandedpolygon.h"

class Evaluation
{
public:
    Evaluation();
    std::vector<std::tuple<int , int , int , int ,int>> evaluation(procon::NeoExpandedPolygon const& frame , procon::NeoExpandedPolygon const& polygon);
};

#endif // EVALUATION_H
