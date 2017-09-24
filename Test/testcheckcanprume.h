#ifndef TESTCHECKCANPRUME_H
#define TESTCHECKCANPRUME_H
#include "testerwraper.h"
#include "neofield.h"

class TestCheckCanPrume : public TesterWraper
{
public:
    TestCheckCanPrume();
    procon::NeoField makeField();
    bool run();
};

#endif // TESTCHECKCANPRUME_H
