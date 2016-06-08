#include <iostream>

#include "mainwindow.h"
#include <QApplication>

#include "testpolygon.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    TestPolygon test_polygon;

    std::cout << "ok." << std::endl;

    return a.exec();
}
