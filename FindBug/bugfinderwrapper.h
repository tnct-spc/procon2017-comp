#ifndef BUGFINDERWRAPPER_H
#define BUGFINDERWRAPPER_H
#include "precompile.h"

#define print(S) std::cout<<"["<<__FILE__<<"]"<<"["<<std::to_string(__LINE__)<<"] "<<S<<std::endl;

class BugFinderWrapper
{
public:
    BugFinderWrapper();
    virtual bool run(){print("マクロあるで");return true;}
};

#endif // BUGFINDERWRAPPER_H
