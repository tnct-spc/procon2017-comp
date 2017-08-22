#ifndef EVALUATION_H
#define EVALUATION_H
#include "neoexpandedpolygon.h"
#include "Utils/polygonconnector.h"

class Evaluation
{
public:
    Evaluation();
    static int calculation_nep (const procon::NeoExpandedPolygon &nep , int index , int cal);
    static std::vector<std::pair<double , Connect>> evaluation(
            procon::NeoExpandedPolygon const& frame ,
            procon::NeoExpandedPolygon const& polygon ,
            double angle_weight = 1.0 ,
            double side_weight = 1.0,
            bool contain_zero = true);
};

#endif // EVALUATION_H
