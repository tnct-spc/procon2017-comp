#ifndef SOLVER_H
#define SOLVER_H

#include "solver_global.h"
#include "field.h"
#include "polygonset.h"

class SOLVERSHARED_EXPORT Solver
{

public:
    Solver();
    Field run(PolygonSet set);
};

#endif // SOLVER_H
