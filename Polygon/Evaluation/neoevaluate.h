#ifndef NEOEVALUATE_H
#define NEOEVALUATE_H

#include "Utils/polygonconnector.h"


class NeoEvaluate
{
public:
    double score = -1;
    Connect connection;
    int frame_index = -1;
    int piece_index = -1;
    int piece_vector_index = -1;
    int fields_index = -1;
    bool is_inversed = false;
};

#endif // NEOEVALUATE_H
