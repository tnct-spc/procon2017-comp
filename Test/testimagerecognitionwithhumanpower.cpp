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

    cv::Mat image = cv::imread("../../../Pictures/k-on.png");

    imagerecongnitionwithhumanpower *iwh = new imagerecongnitionwithhumanpower();

    QObject::connect(iwh,&imagerecongnitionwithhumanpower::returnPolygon,[](polygon_t returnpolygon){
           std::cout << "called" << std::endl;

           std::cout << boost::geometry::dsv(returnpolygon) << std::endl;
    });

    iwh->setPolygon(polygon);
//    iwh->setImage("../../procon2017-comp/background.png");
    iwh->setImage(image);
    iwh->show();
//    iwh->setPolygon(polygon);
    std::cout<<"hello"<<std::endl;
    return 1;
}
