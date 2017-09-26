#include "neoexpandedpolygon.h"
#include "neopolygonviewer.h"
#include "testneoexpandedpolygon.h"

TestNeoExpandedPolygon::TestNeoExpandedPolygon()
{

}

bool TestNeoExpandedPolygon::run()
{
    auto testpoly = [](polygon_i poly, int n) {
        NeoPolygonViewer::getInstance().displayPolygon(poly,"poly " + std::to_string(n),false);

        procon::NeoExpandedPolygon expoly;

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

        std::cout << "---------------------------------------<< poly " << n << " >>---------------------------------------" << std::endl << std::endl;
        expoly.resetPolygonForce(poly);
        out(expoly, "polygon");

        out_int(expoly.getSize(), "size");
        out_double(expoly.getSideLength(), "length");
        out_double(expoly.getSideAngle(), "angle");
        out_double(expoly.getSideSlope(), "slope");
        expoly.inversePolygon();
        out(expoly, "inversePolygon()");
        expoly.translatePolygon(1, 1);
        out(expoly, "translatePolygon(1, 1)");
        expoly.setPolygonPosition(0, 0);
        out(expoly, "setPolygonPosition(0, 0)");

        std::cout << std::endl;
    };

    std::vector<polygon_i> polygons{{{{1, 1}, {1, 3}, {3, 3}, {3, 8}, {4, 8}, {4, 1}, {1, 1}}},
                                    {{{1, 4}, {0, 5}, {2, 6}, {1, 9}, {1, 10}, {4, 10}, {5, 8}, {4, 5}, {5, 2}, {4, 2}, {3, 5}, {2, 3}, {3, 2}, {1, 2}, {1, 4}}}
                                   };
    int n = 1;

    for(polygon_i poly : polygons) {
        testpoly(poly, n);
        ++n;
    }
    return true;
}
