#ifndef SIMPLEALGORITHM_H
#define SIMPLEALGORITHM_H

#include "algorithmwrapper.h"

class SimpleAlgorithm : public AlgorithmWrapper
{
public:
    SimpleAlgorithm();
    void run(procon::Field field);
};

#endif // SIMPLEALGORITHM_H
