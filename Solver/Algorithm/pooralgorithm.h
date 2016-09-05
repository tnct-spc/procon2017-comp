#ifndef POORALGORITHM_H
#define POORALGORITHM_H

#include "algorithmwrapper.h"
#include "singlepolygondisplay.h"

class PoorAlgorithm : public AlgorithmWrapper
{
public:
    PoorAlgorithm();
    procon::Field run(procon::Field field);

    SinglePolygonDisplay Displayyy;

};

#endif // POORALGORITHM_H
