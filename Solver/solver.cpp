#include "solver.h"

#include "Algorithm/pooralgorithm.h"
#include "Algorithm/simplealgorithm.h"
#include "Algorithm/hillclibming.h"
#include "Algorithm/beamsearch.h"

Solver::Solver()
{
}

procon::Field Solver::run(procon::Field field, int algorithm_number)
{
    std::vector<AlgorithmWrapper*> Algorithms;
    Algorithms.push_back(new PoorAlgorithm());
    Algorithms.push_back(new SimpleAlgorithm());
    Algorithms.push_back(new HillClibming());
    Algorithms.push_back(new BeamSearch());

    procon::Field result = Algorithms.at(algorithm_number)->run(field);

    for(auto algo : Algorithms) delete(algo);
    Algorithms.clear();

    return result;
}

