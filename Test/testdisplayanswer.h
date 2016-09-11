#ifndef TESTDISPLAYANSWER_H
#define TESTDISPLAYANSWER_H
#include "stdio.h"
#include "testerwraper.h"
class TestDisplayAnswer : public TesterWraper
{
public:
    TestDisplayAnswer();
    bool run();
};

#endif // TESTDISPLAYANSWER_H
