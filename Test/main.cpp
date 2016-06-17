#include <iostream>

#include "mainwindow.h"
#include <QApplication>

#include "testpolygon.h"
#include "testdisplayanswer.h"

bool Test()
{
    TestPolygon test_polygon;
    TestDisplayAnswer test_displayanswer;

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
