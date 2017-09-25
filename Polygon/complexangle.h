#ifndef COMPLEXANGLE_H
#define COMPLEXANGLE_H

#include "precompile.h"

class ComplexAngle
{
public:
    int x;
    int y;
    ComplexAngle(int x, int y);
    ComplexAngle operator /(ComplexAngle angle);
    ComplexAngle operator =(ComplexAngle angle);
    bool operator ==(ComplexAngle angle);
    double angle();
    static double angle(ComplexAngle angle);
};

#endif // COMPLEXANGLE_H
