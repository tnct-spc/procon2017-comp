#ifndef TESTCHECKODDFIELD_H
#define TESTCHECKODDFIELD_H

#include "testerwraper.h"
#include "neopolygonviewer.h"
#include "neofield.h"
#include "Utils/polygonconnector.h"

class testCheckOddField : public TesterWraper
{
public:
    testCheckOddField();
    bool run();
};

#endif // TESTCHECKODDFIELD_H
