#include "solver.h"

#include "singlepolygondisplay.h"
#include "Algorithm/pooralgorithm.h"
#include "Algorithm/simplealgorithm.h"
#include "Algorithm/hillclibming.h"
#include "Algorithm/beamsearch.h"
#include "Algorithm/beamsearchbylength.h"

Solver::Solver()
{
}

void Solver::run(procon::Field field, int algorithm_number)
{
    std::vector<AlgorithmWrapper*> Algorithms;
    Algorithms.push_back(new PoorAlgorithm());
    Algorithms.push_back(new SimpleAlgorithm());
    Algorithms.push_back(new HillClibming());
    Algorithms.push_back(new BeamSearch());
    Algorithms.push_back(new BeamSearchByLength());

    connect(Algorithms.at(algorithm_number),&PoorAlgorithm::throwAnswer,this,&Solver::emitAnswer);
    Algorithms.at(algorithm_number)->run(field);

    for(auto algo : Algorithms) delete(algo);
    Algorithms.clear();

    return;
}

void Solver::emitAnswer(procon::Field field)
{
    emit throwAnswer(field);
}
