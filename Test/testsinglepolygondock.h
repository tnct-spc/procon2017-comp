#ifndef TESTSINGLEPOLYGONDOCK_H
#define TESTSINGLEPOLYGONDOCK_H
#include "neofield.h"
#include "singlepolygondock.h"
#include "neopolygonviewer.h"
#include "neoexpandedpolygon.h"
#include "neopolygonio.h"
#include "testerwraper.h"

class TestSinglePolygonDock : public TesterWraper{
public:
    TestSinglePolygonDock();
    bool run();
};

#endif // TESTSINGLEPOLYGONDOCK_H
