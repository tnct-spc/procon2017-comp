#include "testimagerecognitionwithhumanpower.h"
#include "imagerecongnitionwithhumanpower.h"
#include "imagerecognition.h"
#include "mygraphicsview.h"
#include "neoanswerboard.h"

#include <QObject>

TestImageRecognitionWithHumanPower::TestImageRecognitionWithHumanPower()
{

}

bool TestImageRecognitionWithHumanPower::run()
{
    cv::Mat frame = cv::imread("../../procon2017-comp/sample/frame_image300.png", 1);
    cv::Mat pieces = cv::imread("../../procon2017-comp/sample/pieces_image300.png", 1);

    ImageRecognition imrec;
    procon::NeoField field = imrec.run(frame, pieces);

    std::cout<<"\ntestend"<<std::endl;
    return 1;
}
