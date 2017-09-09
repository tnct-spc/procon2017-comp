#include "neosolver.h"
#include "Algorithm/algorithmwrapper.h"
#include "Algorithm/testalgortithm.h"
#include "Algorithm/beamsearch.h"

NeoSolver::NeoSolver()
{
    logger = spdlog::get("Solver");
}

NeoSolver::~NeoSolver()
{

}

void NeoSolver::run(procon::NeoField field, int algorithm_number)
{
    Algorithms.push_back(new TestAlgortithm());
    Algorithms.push_back(new BeamSearch());

    field.setFrame(field.getElementaryFrame());
    emitAnswer(field);

    connect(Algorithms.at(algorithm_number),&AlgorithmWrapper::throwAnswer,this,&NeoSolver::emitAnswer);
    Algorithms.at(algorithm_number)->init();
    Algorithms.at(algorithm_number)->run(field);

    std::cout << "neosolver " << std::endl;

    return;
}

void NeoSolver::emitAnswer(procon::NeoField field){
    std::cout << "neoalgrotithm warpper throwanswer called" << std::endl;
    emit throwAnswer(field);
}
