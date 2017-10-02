#include "testimagerecognitionwithhumanpower.h"
#include "imagerecongnitionwithhumanpower.h"

#include <QObject>

TestImageRecognitionWithHumanPower::TestImageRecognitionWithHumanPower()
{

}

bool TestImageRecognitionWithHumanPower::run()
{
    polygon_t polygon;
    bg::exterior_ring(polygon) = boost::assign::list_of<point_t>(0.5, 0.5)(0.1, 3.0)(3.0, 3.1)(0.5, 0.5);
    std::cout << "polygon" << bg::dsv(polygon) << std::endl;

//    imagerecongnitionwithhumanpower *irwh = new imagerecongnitionwithhumanpower();
//    irwh->show();
    return 1;
}
