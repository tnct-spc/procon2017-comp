#include <iostream>

#include "mainwindow.h"
#include <QApplication>

#include "testpolygon.h"

bool Test()
{
    TestPolygon test_polygon;

    if(!test_polygon.run()){
        std::cout<<"*****Polygon error*****"<<std::endl;
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
