#include "neoexpandedpolygon.h"
#include "testneoexpandedpolygon.h"

TestNeoExpandedPolygon::TestNeoExpandedPolygon()
{

}

bool TestNeoExpandedPolygon::run()
{
    polygon_i poly1{{{1, 1}, {1, 3}, {3, 3}, {3, 8}, {4, 8}, {4, 1}, {1, 1}}};
    procon::NeoExpandedPolygon expoly1;

    auto out = [](procon::NeoExpandedPolygon expoly, std::string name)
    {
        polygon_i poly = expoly.getPolygon();
        point_i point;

        std::cout << name << " -> ";

        for(auto itr : poly.outer()) {
            point = itr;
            std::cout << "(" << point.x() << ", " << point.y() << "), ";
        }

        std::cout << std::endl << std::endl;
    };

    auto out_int = [](int a, std::string name)
    {
        std::cout << name << " -> " << a << std::endl << std::endl;
    };

    auto out_double = [](std::vector<double> b, std::string name)
    {
        std::cout << name << " -> " << std::endl;

        for(auto itr : b) {
            std::cout << itr << ", ";
        }
        std::cout << std::endl << std::endl;
    };

    expoly1.resetPolygonForce(poly1);
    out(expoly1, "polygon");

    out_int(expoly1.getSize(), "size");
    out_double(expoly1.getSideLength(), "length");
    out_double(expoly1.getSideAngle(), "angle");
    out_double(expoly1.getSideSlope(), "slope");
    expoly1.inversePolygon();
    out(expoly1, "inversePolygon()");
    expoly1.translatePolygon(1, 1);
    out(expoly1, "translatePolygon(1, 1)");
    expoly1.setPolygonPosition(0, 0);
    out(expoly1, "setPolygonPosition(0, 0)");
    return true;
}
