#include <iostream>
#include <QApplication>
#include "utilities.h"

#include "testdisplayanswer.h"
#include "testjoinpolygon.h"

int Test()
{
    int ERROR_CNT = 0;

    TestDisplayAnswer test_displayanswer;
    TestJoinPolygon test_joinpolygon;

    if(!test_displayanswer.run()){
        std::cout<<"*****Polygon error*****"<<std::endl;
        ERROR_CNT++;
    }
    if(!test_joinpolygon.run()){
        std::cout<<"ro"<<std::endl;
        ERROR_CNT++;
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
