#include "solver.h"

#include "Algorithm/pooralgorithm.h"
#include "Algorithm/simplealgorithm.h"

Solver::Solver()
{
}

procon::Field Solver::run(PolygonSet set)
{
    PoorAlgorithm algo1;
    SimpleAlgorithm algo2;

    procon::Field field = algo1.run(set);

    return field;
}
