#include "checksomething.h"
#include "neoexpandedpolygon.h"

checkSomething::checkSomething()
{

}

void checkSomething::check_calcSideAngle()
{
    std::vector<polygon_i> polygons{{{{5, 5}, {15, 8}, {7, 2}, {15, 8}, {10, 4}, {16, 8}, {16, 15}, {16, 20}, {5, 24}, {5, 5}}}
                                   };
    int i = 0;
    for(polygon_i polygon : polygons) {
        procon::NeoExpandedPolygon neoPolygon;
        neoPolygon.resetPolygonForce(polygon);
        std::vector<double> angles = neoPolygon.getSideAngle();
        std::cout << "polygon <" << i << ">" << std::endl;
        for(double angle : angles) {
            std::cout << angle << ", ";
        }
        std::cout << std::endl << std::endl;
        if(angles.at(2) == 0.0) std::cout << "0" << std::endl;
        if(angles.at(6) == M_PI) std::cout << "M_PI" << std::endl;
        ++i;
    }
}

bool checkSomething::run()
{
    check_calcSideAngle();
    return true;
}
