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
    std::string path = "./../../procon2016-comp/picture/scan.png";

    //前処理
    std::vector<cv::Mat> images = Preprocessing(path);

    //線分検出
    std::vector<std::vector<cv::Vec4f>> pieces_lines = LineDetection(images);

    //ベクター化
    std::vector<PolygonExpansion> polygons = Vectored(pieces_lines);

    cv::waitKey();
}

std::vector<cv::Mat> ImageRecognition::Preprocessing(std::string const& path)
{
    //画像読み込み
    cv::Mat image = cv::imread(path, 1);
    int rows = image.rows;
    int cols = image.cols;

    //色抽出 H:0-255/180, S:51-255/255, B:133-230/255
    colorExtraction(&image, &image, CV_BGR2HSV, 0, 180, 51, 255, 133, 191);

    //グレースケールに変換
    cvtColor(image,image,CV_RGB2GRAY);

    //二値化
    cv::threshold(image, image, 0, 255, cv::THRESH_BINARY_INV);

    //ピース内に混じっている白い穴を削除
    cv::Mat *hole_label = new cv::Mat();
    cv::connectedComponents(image, *hole_label,4);
    for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++) image.at<unsigned char>(y,x) = hole_label->at<int>(y,x) == 1  ? 255 : 0;
    delete hole_label;

    //二値化
    cv::threshold(image, image, 0, 255, cv::THRESH_BINARY_INV);

    //ピースを一つ一つに分ける
    struct minmax2D{
        int minX = 10000;
        int maxX = 0;
        int minY = 10000;
        int maxY = 0;
    };
    std::vector<cv::Mat> images;
    cv::Mat *piece_label = new cv::Mat();
    int label_num = cv::connectedComponents(image, *piece_label);
    std::vector<struct minmax2D> minmaxs(label_num);
    for(int i=0;i<label_num;++i) images.push_back(cv::Mat(rows,cols,CV_8UC1));
    int n;
    for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++)
    {
        n = piece_label->at<int>(y,x);
        if(n!=0){
            images[n-1].at<unsigned char>(y,x) = 255;
            if(minmaxs[n-1].minX > x) minmaxs[n-1].minX = x;
            if(minmaxs[n-1].maxX < x) minmaxs[n-1].maxX = x;
            if(minmaxs[n-1].minY > y) minmaxs[n-1].minY = y;
            if(minmaxs[n-1].maxY < y) minmaxs[n-1].maxY = y;
        }
    }
    delete piece_label;

    //triming, and delete small noise
    const int NOIZE_SIZE = 10;
    std::vector<cv::Mat> result_images;
    int count=0;
    for(auto &im: images){
        if(minmaxs[count].maxX-minmaxs[count].minX < NOIZE_SIZE && minmaxs[count].maxY-minmaxs[count].minY < NOIZE_SIZE){
            count++;
            continue;
        }
        result_images.push_back(cv::Mat(im,cv::Rect(minmaxs[count].minX - 5 < 0 ? 0 : minmaxs[count].minX - 5,
                                                    minmaxs[count].minY - 5 < 0 ? 0 : minmaxs[count].minY - 5,
                                                    minmaxs[count].maxX + 5 > cols ? cols-minmaxs[count].minX : minmaxs[count].maxX-minmaxs[count].minX + 10,
                                                    minmaxs[count].maxY + 5 > rows ? rows-minmaxs[count].minY : minmaxs[count].maxY-minmaxs[count].minY + 10
                                                    )));
        count++;
    }

    //二値化
    for(auto &im : images){
        cv::threshold(im, im, 0, 255, cv::THRESH_BINARY_INV);
    }

    return std::move(result_images);
}

std::vector<std::vector<cv::Vec4f>> ImageRecognition::LineDetection(std::vector<cv::Mat> const& images)
{
    std::vector<std::vector<cv::Vec4f>> pieces_lines;

    int count = 0;
    for(auto &image : images){
        pieces_lines.push_back(std::vector<cv::Vec4f>());

        //LSD直線検出 引数の"scale"が重要！！！
        cv::Ptr<cv::LineSegmentDetector> lsd = cv::createLineSegmentDetector(cv::LSD_REFINE_STD,0.35);
        lsd->detect(image, pieces_lines[count]);

        //描画
        cv::Mat pic(image);
        lsd->drawSegments(pic, pieces_lines[count]);
        cv::namedWindow(std::to_string(count+1),CV_WINDOW_NORMAL);
        cv::imshow(std::to_string(count+1), pic);

        count++;
    }

    return std::move(pieces_lines);
}

std::vector<PolygonExpansion> ImageRecognition::Vectored(std::vector<std::vector<cv::Vec4f>> const& pieces_lines)
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
