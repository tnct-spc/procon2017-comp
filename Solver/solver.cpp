#include "solver.h"

#include "Algorithm/pooralgorithm.h"
#include "Algorithm/simplealgorithm.h"

Solver::Solver()
{
}

procon::Field Solver::run(procon::Field field)
{
    PoorAlgorithm poor_algo;
    SimpleAlgorithm simple_algo;

    procon::Field poor_field = poor_algo.run(field);
    procon::Field simple_field = simple_algo.run(field);

    return simple_field;
}
