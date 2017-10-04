#include "neosolver.h"
#include "Algorithm/algorithmwrapper.h"
#include "Algorithm/testalgortithm.h"
#include "Algorithm/beamsearch.h"

NeoSolver::NeoSolver()
{
    logger = spdlog::get("Solver");
    Algorithms.push_back(new TestAlgortithm());
    Algorithms.push_back(new BeamSearch(beamwidth));
    for(auto& al : Algorithms){
//        connect(al, &AlgorithmWrapper::requestCSV, this, &NeoSolver::_requestCSV);
//        connect(this, SIGNAL(requestCSVcomplete()), al, SLOT(_requestCSVcomplete()));
//        connect(al, &AlgorithmWrapper::requestpostCSV, this, &NeoSolver::_requestCSV);
//        connect(this, SIGNAL(requestCSVcomplete()), al, SLOT(_requestCSVcomplete()));
    }
}

NeoSolver::NeoSolver(int beam_width)
{
    logger = spdlog::get("Solver");
    Algorithms.push_back(new TestAlgortithm());
    Algorithms.push_back(new BeamSearch(beam_width));
    for(auto& al : Algorithms){
//        connect(al, &AlgorithmWrapper::requestCSV, this, &NeoSolver::_requestCSV);
//        connect(this, SIGNAL(requestCSVcomplete()), al, SLOT(_requestCSVcomplete()));
//        connect(al, &AlgorithmWrapper::requestpostCSV, this, &NeoSolver::_requestCSV);
//        connect(this, SIGNAL(requestCSVcomplete()), al, SLOT(_requestCSVcomplete()));
    }
}

NeoSolver::NeoSolver(int beam_width, bool answer_progress_enabled)
{
    logger = spdlog::get("Solver");
    Algorithms.push_back(new TestAlgortithm());
    Algorithms.push_back(new BeamSearch(beam_width,answer_progress_enabled));
    for(auto& al : Algorithms){
//        connect(al, &AlgorithmWrapper::requestCSV, this, &NeoSolver::_requestCSV);
//        connect(this, SIGNAL(requestCSVcomplete()), al, SLOT(_requestCSVcomplete()));
//        connect(al, &AlgorithmWrapper::requestpostCSV, this, &NeoSolver::_requestCSV);
//        connect(this, SIGNAL(requestCSVcomplete()), al, SLOT(_requestCSVcomplete()));
    }
}

NeoSolver::~NeoSolver()
{

}

void NeoSolver::run(procon::NeoField field, int algorithm_number)
{

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

void NeoSolver::_requestCSV()
{
    emit requestCSV();
}

void NeoSolver::_requestCSVcomplete()
{
    emit requestCSVcomplete();
}

void NeoSolver::_requestpostCSV()
{
    emit requestpostCSV();
}

void NeoSolver::_requestpostCSVcomplete()
{
    emit requestpostCSVcomplete();
}
