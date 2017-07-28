#include "neosolver.h"
#include "Algorithm/testalgortithm.h"

NeoSolver::NeoSolver()
{
}

NeoSolver::~NeoSolver()
{

}

void NeoSolver::run(procon::NeoField field, int algorithm_number)
{
    Algorithms.push_back(new TestAlgortithm());


}
