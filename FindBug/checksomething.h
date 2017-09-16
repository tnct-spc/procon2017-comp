#ifndef CHECKSOMETHING_H
#define CHECKSOMETHING_H

#include "bugfinderwrapper.h"

class checkSomething : public BugFinderWrapper
{
public:
    checkSomething();
    void check_calcSideAngle();
    bool run();
};

#endif // CHECKSOMETHING_H
