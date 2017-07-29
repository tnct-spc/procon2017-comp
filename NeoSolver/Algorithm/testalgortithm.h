#ifndef TESTALGORTITHM_H
#define TESTALGORTITHM_H

#include "algorithmwrapper.h"

class TestAlgortithm : public AlgorithmWrapper
{
public:
    TestAlgortithm();
    void init();
    void run(procon::NeoField field);
};

#endif // TESTALGORTITHM_H
