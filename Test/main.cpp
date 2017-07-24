#include <iostream>
#include <QApplication>
#include "utilities.h"

#include "testneoexpandedpolygon.h"
#include "testdisplayanswer.h"
#include "testjoinpolygon.h"
#include "testsearchsamelength.h"

int Test()
{
    int ERROR_CNT = 0;

    std::vector<TesterWraper*> TESTER;
//    TESTER.push_back(new TestNeoExpandedPolygon);
//    TESTER.push_back(new TesterWraper);
//    TESTER.push_back(new TestDisplayAnswer);
//    TESTER.push_back(new TestJoinPolygon);
//    TESTER.push_back(new testSearchSameLength);

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

    int error_cnt = Test();

    if(error_cnt == 0){
        std::cout << "***ALL SUCCESS!!!***" << std::endl;
    } else {
        std::cout << "***FAILED! "<<error_cnt<<" tests return false***" << std::endl;
    }
    return a.exec();
}
