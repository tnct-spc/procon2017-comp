#ifndef UTILITIES_H
#define UTILITIES_H

#define TO_STRING(VariableName) # VariableName

#include "utilities_global.h"
#include <cmath>
#include <iostream>

#define debugprint(S); std::cout<<"["<<__FILE__<<"]"<<"["<<std::to_string(__LINE__)<<"] "<<S<<std::endl;

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
