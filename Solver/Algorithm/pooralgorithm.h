#ifndef POORALGORITHM_H
#define POORALGORITHM_H

#include "algorithmwrapper.h"

class PoorAlgorithm : public AlgorithmWrapper
{
public:
    PoorAlgorithm();
    Field run(PolygonSet set);
};

#endif // POORALGORITHM_H
