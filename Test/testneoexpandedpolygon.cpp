#include "neoexpandedpolygon.h"
#include "testneoexpandedpolygon.h"

TestNeoExpandedPolygon::TestNeoExpandedPolygon()
{

}

bool TestNeoExpandedPolygon::run()
{
    polygon_i poly1;
    poly1.outer() = {(1, 1), (1, 3), (3, 3), (3, 8), (4, 8), (4, 1), (1, 1)};
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

        std::cout << std::endl;
    };

    auto out = [](std::vector<procon::NeoExpandedPolygon> expolies, std::string name)
    {
        std::cout << name << " -> " << std::endl;

        for(auto itr : expolies) {
            procon::NeoExpandedPolygon expoly = itr;
            polygon_i poly = expoly.getPolygon();
            point_i point;

            for(auto itr1 : poly.outer()) {
                point = itr1;
                std::cout << "(" << point.x() << ", " << point.y() << "), ";
            }

            std::cout << std::endl;
        }
    };

    expoly1.resetPolygonForce(poly1);

    std::cout << "getID" << " -> " << expoly1.getId() << std::endl;
    std::cout << "getJointedPieces" << " -> "outexpoly1.getJointedPieces(), ;
    out(expoly1.getMultiIds(), "getMulti");
    out(expoly1.inversePolygon(), "inversePolygon");
    out(expoly1,)
}
