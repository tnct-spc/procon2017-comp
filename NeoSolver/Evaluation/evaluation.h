#ifndef EVALUATION_H
#define EVALUATION_H
#include "neoexpandedpolygon.h"

class Evaluation
{
public:
    Evaluation();
    std::vector<std::tuple<int , int , int , int>> evaluation(std::vector<procon::NeoExpandedPolygon> field, procon::NeoExpandedPolygon polygon);
};

#endif // EVALUATION_H
