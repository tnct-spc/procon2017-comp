#ifndef HILLCLIBMING_H
#define HILLCLIBMING_H

#include "algorithmwrapper.h"

class HillClibming : public AlgorithmWrapper
{
public:
    HillClibming();
    procon::Field run(procon::Field field);
};

#endif // HILLCLIBMING_H
