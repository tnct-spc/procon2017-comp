#ifndef TESTERWRAPER_H
#define TESTERWRAPER_H

#define print(S) std::cout<<"["<<__FILE__<<"]"<<"["<<std::to_string(__LINE__)<<"] "<<S<<std::endl;

class TesterWraper
{
public:
    TesterWraper();
    virtual bool run(){print("マクロあるで");return true;}
};

#endif // TESTERWRAPER_H
