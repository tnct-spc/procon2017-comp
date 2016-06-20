#include <QCoreApplication>
#include <boost/geometry/geometries/multi_polygon.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/algorithms/area.hpp>
#include <boost/geometry.hpp>
#include <boost/assign/list_of.hpp>
#include <cmath>
#include <vector>
#include <iostream>
#include "field.h"
#include "polygonexpansion.h"

int main(int argc,char *argv[])
{
    /*polygon*/
    QCoreApplication a(argc, argv);
    std::cout << "Hello,polygon!" << std::endl;
    polygon_t b,c,d;
    bg::exterior_ring(b) = boost::assign::list_of<point_t>(0, 0)(30, 0)(30,30)(0, 30)(0, 2)(2, 2)(2,28)(28, 28)(28, 2)(0, 2)(0,0);
    bg::exterior_ring(c) = boost::assign::list_of<point_t>(0, 0)(0,3)(3,3)(3,0)(0,0);
    Field hoge;
    hoge.setFlame(b);
        std::cout << "Hello,polygon!" << std::endl;
    hoge.pushPiece(c);
        std::cout << "Hello,polygon!" << std::endl;
    hoge.setPiece(c,1);
        std::cout << "Hello,polygon!" << std::endl;
        std::cout << bg::dsv(hoge.getFlame()) << std::endl;
        std::cout << bg::dsv(hoge.getPiece(0)) << std::endl;
        std::cout << hoge.pieceSize() << std::endl;
    hoge.printFlame();
    hoge.printPiece();
    PolygonExpansion pol;
    pol.setPolygon(c);
    auto s = pol.getSideLength();
    std::for_each(s.begin(),s.end(),[](auto n){std::cout << n << std::endl;});
    return a.exec();
}
