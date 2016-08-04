#ifndef SIMPLEALGORITHM_H
#define SIMPLEALGORITHM_H

#include "algorithmwrapper.h"

class SimpleAlgorithm : public AlgorithmWrapper
{
public:
    SimpleAlgorithm();
    procon::Field run(PolygonSet set);
};

#endif // SIMPLEALGORITHM_H
