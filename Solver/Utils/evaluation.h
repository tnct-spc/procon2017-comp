#ifndef EVALUATION_H
#define EVALUATION_H

#include "Utils/fit.h"
#include <array>

class Evaluation
{
public:
    Evaluation();
    std::array<Fit,2> fits;
    int piece_id;
    int frame_id;
    double evaluation;
};

#endif // EVALUATION_H
