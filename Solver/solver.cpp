#include "solver.h"

#include "Algorithm/pooralgorithm.h"
#include "Algorithm/simplealgorithm.h"

Solver::Solver()
{
}

Field Solver::run(PolygonSet set)
{
    PoorAlgorithm algo1;
    SimpleAlgorithm algo2;

    Field field = algo1.run(set);

    return field;
}
