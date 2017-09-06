#include <QApplication>
#include "bugfinderwrapper.h"
#include "findnoelement.h"
#include "neopolygonviewer.h"
#include "checksomething.h"

int Test()
{
    int ERROR_CNT = 0;

    std::vector<BugFinderWrapper*> TESTER;
    //TESTER.push_back(new FindNoElement);
    TESTER.push_back(new checkSomething);

    for(auto tester : TESTER){
        if(!tester->run()){
            std::cout<<"エラー!"<<std::endl;
            ERROR_CNT++;
        }
    }

    return ERROR_CNT;
}


int main(int argc, char *argv[])
{
    std::cout<<"***START TEST***"<<std::endl;
    QApplication a(argc, argv);
    NeoPolygonViewer::getInstance();

    int error_cnt = Test();

    if(error_cnt == 0){
        std::cout << "***ALL SUCCESS!!!***" << std::endl;
    } else {
        std::cout << "***FAILED! "<<error_cnt<<" tests return false***" << std::endl;
    }
    return a.exec();
}
