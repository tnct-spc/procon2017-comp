#ifndef TESTSINGLEPOLYGONDOCK_H
#define TESTSINGLEPOLYGONDOCK_H
#include "singlepolygondisplay.h"
#include "field.h"
#include "neopolygonviewer.h"
#include "neoexpandedpolygon.h"


class TestSinglePolygonDock : public TesterWraper
{
public:
    TestSinglePolygonDock();
    bool run();
};

#endif // TESTSINGLEPOLYGONDOCK_H
