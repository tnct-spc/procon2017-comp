#ifndef TESTPOLYGONCONNECTOR_H
#define TESTPOLYGONCONNECTOR_H

#include "testerwraper.h"
#include "Utils/polygonconnector.h"
#include "neopolygonviewer.h"

class TestPolygonConnector : public TesterWraper
{
public:
    TestPolygonConnector();
    bool run();
};

#endif // TESTPOLYGONCONNECTOR_H
