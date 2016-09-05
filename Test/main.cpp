#include <iostream>

#include "mainwindow.h"
#include <QApplication>

#include "testpolygon.h"
#include "testdisplayanswer.h"
#include "utility.h"
bool Test()
{
    TestPolygon test_polygon;
    TestDisplayAnswer test_displayanswer;

    double e = 0.0,b = 0.5,c = - 0.7,d = 0.123;
    std::cout << procon::nearlyEqual(e,b,0.6) << std::endl;
    std::cout << procon::nearlyEqual(e,c,0.8) << std::endl;
    std::cout << procon::nearlyEqual(b,c,2) << std::endl;
    std::cout << procon::nearlyEqual(e,c,0.5) << std::endl;
    std::cout << procon::floor(d,0) << std::endl;
    std::cout << procon::floor(d,1) << std::endl;
    std::cout << procon::floor(d,2) << std::endl;
    std::cout << procon::floor(d,3) << std::endl;


    if(!test_polygon.run()){
        std::cout<<"*****Polygon error*****"<<std::endl;
        return false;
    }
    if(!test_displayanswer.run()){
        std::cout<<"*****Display error*****"<<std::endl;
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    if(Test()){
        std::cout << "ok" << std::endl;
    }else{
        std::cout << "error" << std::endl;
    }

    return a.exec();
}
