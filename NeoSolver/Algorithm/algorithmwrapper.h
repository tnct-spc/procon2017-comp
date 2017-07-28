#ifndef ALGORITHMWRAPPER_H
#define ALGORITHMWRAPPER_H

#include "neofield.h"

class AlgorithmWrapper
{
public:
    AlgorithmWrapper();
    ~AlgorithmWrapper();
    void init();
    void run();
    void submitAnswer(procon::NeoField field);

signals:
    void throwAnswer(procon::NeoField field);
};

#endif // ALGORITHMWRAPPER_H
