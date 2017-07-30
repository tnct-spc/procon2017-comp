#ifndef TESTALGORTITHM_H
#define TESTALGORTITHM_H

#include "algorithmwrapper.h"

class TestAlgortithm : public AlgorithmWrapper
{
public:
    TestAlgortithm();
    void init() override;
    void run(procon::NeoField field) override;
};

#endif // TESTALGORTITHM_H
