#include "solver.h"

#include "singlepolygondisplay.h"
#include "Algorithm/pooralgorithm.h"
#include "Algorithm/simplealgorithm.h"
#include "Algorithm/hillclibming.h"
#include "Algorithm/beamsearch.h"
#include "Algorithm/beamsearchbylength.h"
#include "Algorithm/stepsearch.h"

Solver::Solver()
{
}

Solver::~Solver()
{
    for(auto algo : Algorithms) delete(algo);
    Algorithms.clear();
}

void Solver::run(procon::Field field, int algorithm_number)
{
    Algorithms.push_back(new PoorAlgorithm());
    Algorithms.push_back(new SimpleAlgorithm());
    Algorithms.push_back(new HillClibming());
    Algorithms.push_back(new BeamSearch());
    Algorithms.push_back(new BeamSearchByLength());
    Algorithms.push_back(new StepSearch());

    connect(Algorithms.at(algorithm_number),&AlgorithmWrapper::throwAnswer,this,&Solver::emitAnswer);
    Algorithms.at(algorithm_number)->init();
    Algorithms.at(algorithm_number)->run(field);

    return;
}

void Solver::emitAnswer(procon::Field field)
{
    emit throwAnswer(field);
}
