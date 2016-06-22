#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "imagerecognition.h"
#include "ui_imagerecognition.h"

void ImageRecognition::run()
{
    //画像を読み込む
    cv::Mat src,image;
    std::string path = "./../../procon2016/picture/scan-sample.png";
    src = cv::imread(path);

    //画像をグレースケールとして読み込み
    image = cv::imread(path, 0);

    cv::threshold(image, image, 50, 255, cv::THRESH_BINARY); //閾値を設定

    //0..none 1..canny edge
#if 1
    cv::Canny(image, image, 50, 200, 3); // Apply canny edge
#endif
    // Create and LSD detector with standard or no refinement.
#if 0
    cv::Ptr<cv::LineSegmentDetector> ls = cv::createLineSegmentDetector(cv::LSD_REFINE_STD);
#else
    cv::Ptr<cv::LineSegmentDetector> ls = cv::createLineSegmentDetector(cv::LSD_REFINE_NONE);
#endif
    std::vector<cv::Vec4f> lines_std;
    // Detect the lines
    ls->detect(image, lines_std);
    // Show found lines
    cv::Mat drawnLines(image);
    ls->drawSegments(drawnLines, lines_std);
    cv::imshow("IR-LSD", drawnLines);
    cv::waitKey();
}
