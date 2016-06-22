#include "testdisplayanswer.h"
#include "displayanswer.h"

TestDisplayAnswer::TestDisplayAnswer()
{
    disp = new DisplayAnswer();
}

bool TestDisplayAnswer::run()
{
    std::cout << "Hello,display!" << std::endl;
    polygon_t b,c,d;
    bg::exterior_ring(b) = boost::assign::list_of<point_t>(0, 0)(30, 0)(30,30)(0, 30)(0, 2)(2, 2)(2,28)(28, 28)(28, 2)(0, 2)(0,0);
    bg::exterior_ring(c) = boost::assign::list_of<point_t>(0, 0)(0,1)(1,0)(0,0);
    Field f;
    PolygonExpansion hoge,huga;
    hoge.setPolygon(b);
    huga.setPolygon(c);
    f.setFlame(hoge);
    f.pushPiece(huga);
    f.setPiece(huga,1919);
    f.printFlame();
    f.printPiece();
    disp->setField(f);
    return true;
}
