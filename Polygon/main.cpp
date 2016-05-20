#include <QCoreApplication>
#include <boost/geometry/geometries/multi_polygon.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/algorithms/area.hpp>
#include <boost/geometry.hpp>
#include <boost/assign/list_of.hpp>
#include <cmath>
#include <vector>
#include <iostream>

int main(int argc,char *argv[])
{
    /*polygon*/
    QCoreApplication a(argc, argv);
    std::cout << "Hello,polygon!" << std::endl;
    return a.exec();
}
