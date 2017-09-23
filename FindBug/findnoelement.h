#ifndef FINDNOELEMENT_H
#define FINDNOELEMENT_H
#include "bugfinderwrapper.h"
#include "neoanswerboard.h"

class FindNoElement : public BugFinderWrapper
{
public:
    FindNoElement();
    bool run();
private:
    std::shared_ptr<NeoAnswerBoard> nab;
};

#endif // FINDNOELEMENT_H
