#ifndef ALGORITHMWRAPPER_H
#define ALGORITHMWRAPPER_H

#include "field.h"

class AlgorithmWrapper
{
public:
    AlgorithmWrapper();
    virtual procon::Field run(procon::Field field);
    int searchSameLength(procon::ExpandedPolygon polygon1 ,procon::ExpandedPolygon polygon2, int& ans1, int& ans2, int& ans3, int& ans4, double& lengrh1, double& length2, double& angleb1, double& angleb2, double& anglea1, double& anglea2);
};

#endif // ALGORITHMWRAPPER_H
