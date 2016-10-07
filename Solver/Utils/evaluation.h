#ifndef EVALUATION_H
#define EVALUATION_H

#include "fit.h"
#include "utilities.h"

class Evaluation
{
public:
    Evaluation();
    std::array<Fit,2> fits;
    int piece_id;
    int frame_id;
    int vector_id;
    bool inverse_flag;
    double evaluation;
#ifdef HYOKA_MODE
    double evaluation_normal;
    double evaluation_angle;
    double evaluation_length;
    double evaluation_history;
    double evaluation_frame;
#endif
};

#endif // EVALUATION_H
