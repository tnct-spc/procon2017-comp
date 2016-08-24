#ifndef ALGORITHMWRAPPER_H
#define ALGORITHMWRAPPER_H

#include "field.h"

class AlgorithmWrapper
{
public:
    typedef struct FIT{
        bool start_dot_or_line;
        int start_id;
        bool end_dot_or_line;
        int end_id;
    } Fit;

    AlgorithmWrapper();
    virtual procon::Field run(procon::Field field);
    Fit fit1,fit2;
    int searchSameLength(procon::ExpandedPolygon polygon1 ,procon::ExpandedPolygon polygon2, Fit fit1, Fit fit2);
};

#endif // ALGORITHMWRAPPER_H
