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
#include "testpolygon.h"
#include "polygonexpansion.h"
#include "polygonset.h"
#include "polygonio.h"

TestPolygon::TestPolygon()
{

}

bool TestPolygon::run(){
    /*polygon*/
    /*
    std::cout << "Hello,polygon!" << std::endl;
    polygon_t b,c,d;
    bg::exterior_ring(b) = boost::assign::list_of<point_t>(0, 0)(30, 0)(30,30)(0, 30)(0, 2)(2, 2)(2,28)(28, 28)(28, 2)(0, 2)(0,0);
    bg::exterior_ring(c) = boost::assign::list_of<point_t>(0, 0)(0,1)(1,0)(0,0);
    procon::Field f;
    PolygonExpansion hoge,huga;
    hoge.setPolygon(b);
    huga.setPolygon(c);
    f.setFlame(hoge);
    f.setPiece(huga);
    f.setPiece(huga,1919);
    f.printFlame();
    f.printPiece();

    PolygonSet s;
    s.fieldFlame = hoge;
    s.fieldPiece.push_back(huga);
    s.fieldPiece.push_back(huga);
    PolygonIO io;
    io.exportPolygon(s,"./../output.csv");

    PolygonSet newS;
    newS = io.importPolygon("./../output.csv");
    Field newF;
    newF.setFlame(newS.fieldFlame);
    newF.pushPiece(newS.fieldPiece.at(0));
    newF.pushPiece(newS.fieldPiece.at(1));
    newF.printFlame();
    newF.printPiece();
    */
    return 1;
}
