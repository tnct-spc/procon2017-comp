#include "testimagerecognitionwithhumanpower.h"
#include "imagerecongnitionwithhumanpower.h"

TestImageRecognitionWithHumanPower::TestImageRecognitionWithHumanPower()
{

}

bool TestImageRecognitionWithHumanPower::run()
{
    imagerecongnitionwithhumanpower *iwh = new imagerecongnitionwithhumanpower();
    iwh->show();
    std::cout<<"hello"<<std::endl;
    return 1;
}
