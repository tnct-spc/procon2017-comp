#ifndef UTILITIES_H
#define UTILITIES_H

#include "utilities_global.h"
#include <cmath>

class UTILITIESSHARED_EXPORT Utilities
{

public:
    Utilities();
};

namespace procon {
    template<typename X,typename Y>
    X floor(X var,Y power)
    {
        return std::floor(var * std::pow(10,power)) / std::pow(10,power);
    }

    template<typename X,typename Y>
    X round(X var,Y power)
    {
        return std::round(var * std::pow(10,power)) / std::pow(10,power);
    }

    template<typename X,typename Y>
    bool nearlyEqual(X var1,X var2,Y allowance)
    {
        return std::abs(var1 - var2) < allowance ? true : false;
    }
}
#endif // UTILITIES_H
