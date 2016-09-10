#ifndef SOLVER_H
#define SOLVER_H

#include "solver_global.h"
#include "singlepolygondisplay.h"
#include "field.h"

class SOLVERSHARED_EXPORT Solver
{

public:
    Solver();
    procon::Field run(procon::Field field, int algorithm_number = 0);



};

#endif // SOLVER_H
