#ifndef FINDNOELEMENT_H
#define FINDNOELEMENT_H
#include "bugfinderwrapper.h"

class FindNoElement : public BugFinderWrapper
{
public:
    FindNoElement();
    bool run();
};

#endif // FINDNOELEMENT_H
