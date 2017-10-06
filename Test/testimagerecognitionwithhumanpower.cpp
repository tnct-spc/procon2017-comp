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
    cv::Mat frame = cv::imread("../../procon2017-comp/sample/Wed_Oct__4_18:33:57_2017_.png", 1);
    cv::Mat pieces = cv::imread("../../procon2017-comp/sample/Wed_Oct__4_18:35:21_2017_.png", 1);

    ImageRecognition *imrec = new ImageRecognition();
    procon::NeoField field = imrec->run(frame, pieces);

    std::cout<<"\ntestend"<<std::endl;
    return 1;
}
