#ifndef SIMPLEALGORITHM_H
#define SIMPLEALGORITHM_H

#include "algorithmwrapper.h"
#include "singlepolygondisplay.h"

class SimpleAlgorithm : public AlgorithmWrapper
{
public:
    SimpleAlgorithm();
    procon::Field run(procon::Field field);
};

#endif // SIMPLEALGORITHM_H
