#ifndef EVALUATE_H
#define EVALUATE_H

//相対パスにしたらビルド通りました
#include "Utils/polygonconnector.h"

class Evaluate
{
public:
    double score = -1;
    Connect connection;
    int frame_index = -1;
    int piece_index = -1;
    int fields_index = -1;
    bool is_inversed = false;
    int pieses_size = -1;
};

#endif // EVALUATE_H
