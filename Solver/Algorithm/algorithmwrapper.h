#ifndef ALGORITHMWRAPPER_H
#define ALGORITHMWRAPPER_H

#include "field.h"
#include "polygonset.h"

class AlgorithmWrapper
{
public:
    AlgorithmWrapper();
    virtual Field run(PolygonSet set);
};

#endif // ALGORITHMWRAPPER_H
