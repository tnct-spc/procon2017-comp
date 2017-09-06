#ifndef EVALUATE_H
#define EVALUATE_H

//相対パスにしたらビルド通りました
#include "../NeoSolver/Utils/polygonconnector.h"

class Evaluate
{
public:
    Evaluate();

    double score = -1;
    Connect connection;
    int frame_index = -1;
    int piece_index = -1;
    int fields_index = -1;
    bool is_inversed = false;
};

#endif // EVALUATE_H
