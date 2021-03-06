#include <iostream>
#include <QApplication>
#include "utilities.h"

#include "testneoexpandedpolygon.h"
#include "testdisplayanswer.h"
#include "testjoinpolygon.h"
#include "testsearchsamelength.h"
#include "neopolygonviewer.h"
#include "testalgorithmwrapper.h"
#include "testevaluation.h"
#include "testpolygonconnector.h"
#include "testcheckoddfield.h"
#include "testcheckcanprume.h"
#include "testqrtranslatetopolygon.h"
#include "testimagerecognitionwithhumanpower.h"
#include "testsinglepolygondock.h"

int Test()
{
    int ERROR_CNT = 0;

    std::vector<TesterWraper*> TESTER;
//    TESTER.push_back(new TestNeoExpandedPolygon)
//    TESTER.push_back(new TestNeoExpandedPolygon);
//    TESTER.push_back(new TesterWraper);
//    TESTER.push_back(new TestDisplayAnswer);
//    TESTER.push_back(new TestJoinPolygon);
//    TESTER.push_back(new testSearchSameLength);
//    TESTER.push_back(new TestAlgorithmWrapper);
//    TESTER.push_back(new TestEvaluation);
//    TESTER.push_back(new TestPolygonConnector);
//    TESTER.push_back(new testCheckOddField);
//    TESTER.push_back(new TestCheckCanPrume);
//    TESTER.push_back(new testCheckOddField);
//    TESTER.push_back(new TestQrTranslateToPolygon);
    TESTER.push_back(new TestImageRecognitionWithHumanPower);
//    TESTER.push_back(new TestSinglePolygonDock);

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
//    return 0;
    return a.exec();
}
