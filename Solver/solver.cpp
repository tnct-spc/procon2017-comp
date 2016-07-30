#include "solver.h"

#include "Algorithm/simplealgorithm.h"

Solver::Solver()
{
}

Field Solver::run(PolygonSet set)
{
    SimpleAlgorithm algo;
    Field field = algo.run(set);
    return field;
}
