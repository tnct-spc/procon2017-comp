 #ifndef ALGORITHMWRAPPER_H
#define ALGORITHMWRAPPER_H

#include "field.h"

class AlgorithmWrapper
{
public:
    AlgorithmWrapper();
    virtual procon::Field run(procon::Field field);
};

#endif // ALGORITHMWRAPPER_H
