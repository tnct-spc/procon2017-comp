#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "polygon.h"
#include "imagerecognition.h"
#include "ui_imagerecognition.h"

void ImageRecognition::run()
{
    std::string path = "./../../procon2016/picture/scan-sample.png";

    //前処理
    cv::Mat& image = Preprocessing(path);

    //線分検出
    std::vector<cv::Vec4f>& lines = LineDetection(image);

    //ベクター化
    std::vector<PolygonExpansion>& polygons = Vectored(lines);

    cv::waitKey();
}

cv::Mat& ImageRecognition::Preprocessing(std::string path)
{
    //画像をグレースケールとして読み込み(カラーで読み込みたい)
    cv::Mat* const image = new cv::Mat(cv::imread(path, 0));

    //閾値を設定して画像を二値化
    cv::threshold(*image, *image, 140, 255, cv::THRESH_BINARY);

    return *image;
}

std::vector<cv::Vec4f>& ImageRecognition::LineDetection(cv::Mat image)
{
    std::vector<cv::Vec4f>* const lines = new std::vector<cv::Vec4f>();

    //エッジ検出（使用すべきか検討必要）
    //cv::Canny(image, image, 50, 200, 3); // Apply canny edge

    //LSD直線検出(モードは要検証)
    cv::Ptr<cv::LineSegmentDetector> lsd = cv::createLineSegmentDetector(cv::LSD_REFINE_STD);
    //cv::Ptr<cv::LineSegmentDetector> lsd = cv::createLineSegmentDetector(cv::LSD_REFINE_NONE);
    lsd->detect(image, *lines);

    //描画
    cv::Mat pic(image);
    lsd->drawSegments(pic, *lines);
    cv::imshow("pic with LSD line", pic);

    return *lines;
}

std::vector<PolygonExpansion>& ImageRecognition::Vectored(std::vector<cv::Vec4f> lines)
{
    std::vector<PolygonExpansion>* const polygon = new std::vector<PolygonExpansion>();
    return *polygon;
}
