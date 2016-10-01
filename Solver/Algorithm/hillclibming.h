#ifndef HILLCLIBMING_H
#define HILLCLIBMING_H

#include "algorithmwrapper.h"

class HillClibming : public AlgorithmWrapper
{
public:
    HillClibming();
    void run(procon::Field field);
};

#endif // HILLCLIBMING_H
