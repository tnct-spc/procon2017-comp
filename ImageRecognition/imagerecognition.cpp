#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/photo/photo.hpp>
#include <opencv2/photo.hpp>
#include "polygon.h"
#include "imagerecognition.h"
//#include "ui_imagerecognition.h"

void ImageRecognition::run()
{
    std::string path = "./../../procon2016-comp/picture/camera-sample.JPEG";

    //前処理
    cv::Mat image = Preprocessing(path);

    //線分検出
    std::vector<cv::Vec4f> lines = LineDetection(image);

    //ベクター化
    std::vector<PolygonExpansion> polygons = Vectored(lines);

    cv::waitKey();
}

cv::Mat ImageRecognition::Preprocessing(std::string const& path)
{
    //画像をカラーで読み込み
    cv::Mat image = cv::imread(path, 1);

    //H:0-255/180, S:51-255/255, B:133-230/255
    colorExtraction(&image, &image, CV_BGR2HSV, 0, 180, 51, 255, 133, 191);

    //グレースケールに変換
    cvtColor(image,image,CV_RGB2GRAY);

    //二値化
    cv::threshold(image, image, 0, 255, cv::THRESH_BINARY_INV);

    return std::move(image);
}

std::vector<cv::Vec4f> ImageRecognition::LineDetection(cv::Mat const& image)
{
    std::vector<cv::Vec4f> lines;

    //エッジ検出（使用すべきか検討必要）
    //cv::Canny(image, image, 50, 200, 3); // Apply canny edge

    //LSD直線検出(モードは要検証)
    cv::Ptr<cv::LineSegmentDetector> lsd = cv::createLineSegmentDetector(cv::LSD_REFINE_STD);
    //cv::Ptr<cv::LineSegmentDetector> lsd = cv::createLineSegmentDetector(cv::LSD_REFINE_NONE);
    lsd->detect(image, lines);

    //描画
    cv::Mat pic(image);
    lsd->drawSegments(pic, lines);
    cv::namedWindow("pic with LSD line",CV_WINDOW_NORMAL);
    cv::imshow("pic with LSD line", pic);

    return std::move(lines);
}

std::vector<PolygonExpansion> ImageRecognition::Vectored(std::vector<cv::Vec4f> const& lines)
{
    std::vector<PolygonExpansion> polygon;
    return std::move(polygon);
}

void ImageRecognition::colorExtraction(cv::Mat* src, cv::Mat* dst,
    int code,
    int ch1Lower, int ch1Upper,
    int ch2Lower, int ch2Upper,
    int ch3Lower, int ch3Upper
    )
{
    cv::Mat colorImage;
    int lower[3];
    int upper[3];

    cv::Mat lut = cv::Mat(256, 1, CV_8UC3);

    cv::cvtColor(*src, colorImage, code);

    lower[0] = ch1Lower;
    lower[1] = ch2Lower;
    lower[2] = ch3Lower;

    upper[0] = ch1Upper;
    upper[1] = ch2Upper;
    upper[2] = ch3Upper;

    for (int i = 0; i < 256; i++){
        for (int k = 0; k < 3; k++){
            if (lower[k] <= upper[k]){
                if ((lower[k] <= i) && (i <= upper[k])){
                    lut.data[i*lut.step+k] = 255;
                }else{
                    lut.data[i*lut.step+k] = 0;
                }
            }else{
                if ((i <= upper[k]) || (lower[k] <= i)){
                    lut.data[i*lut.step+k] = 255;
                }else{
                    lut.data[i*lut.step+k] = 0;
                }
            }
        }
    }

    //LUTを使用して二値化
    cv::LUT(colorImage, lut, colorImage);

    //Channel毎に分解
    std::vector<cv::Mat> planes;
    cv::split(colorImage, planes);

    //マスクを作成
    cv::Mat maskImage;
    cv::bitwise_and(planes[0], planes[1], maskImage);
    cv::bitwise_and(maskImage, planes[2], maskImage);

    //出力
    cv::Mat maskedImage;
    src->copyTo(maskedImage, maskImage);
    *dst = maskedImage;
}
