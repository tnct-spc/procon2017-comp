#ifndef BEAMSEARCH_H
#define BEAMSEARCH_H

#include "field.h"
#include "Utils/evaluation.h"
#include "Utils/polygonconnector.h"
#include "algorithmwrapper.h"
#include <queue>
#include <algorithm>

class BeamSearch : public AlgorithmWrapper
{
protected:
    std::vector<Evaluation> fieldSearch(procon::Field field);
public:
    BeamSearch();
    procon::Field run(procon::Field field);
};


#endif // BEAMSEARCH_H
