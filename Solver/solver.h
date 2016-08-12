#ifndef SOLVER_H
#define SOLVER_H

#include "solver_global.h"
#include "field.h"
#include "polygonset.h"

class SOLVERSHARED_EXPORT Solver
{

public:
    Solver();
    procon::Field run(procon::Field field);
};

#endif // SOLVER_H
