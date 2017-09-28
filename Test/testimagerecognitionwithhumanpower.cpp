#include "testimagerecognitionwithhumanpower.h"
#include "imagerecongnitionwithhumanpower.h"

TestImageRecognitionWithHumanPower::TestImageRecognitionWithHumanPower()
{

}

bool TestImageRecognitionWithHumanPower::run()
{
    polygon_t polygon;
    bg::exterior_ring(polygon) = boost::assign::list_of<point_t>(0.5, 0.5)(0.1, 3.0)(3.0, 3.1)(0.5, 0.5);

    imagerecongnitionwithhumanpower *iwh = new imagerecongnitionwithhumanpower();
    iwh->setPolygon(polygon);
    iwh->show();
    std::cout<<"hello"<<std::endl;
    return 1;
}
