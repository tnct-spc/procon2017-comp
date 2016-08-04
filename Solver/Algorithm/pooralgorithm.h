#ifndef POORALGORITHM_H
#define POORALGORITHM_H

#include "algorithmwrapper.h"

class PoorAlgorithm : public AlgorithmWrapper
{
public:
    PoorAlgorithm();
    procon::Field run(PolygonSet set);
};

#endif // POORALGORITHM_H
