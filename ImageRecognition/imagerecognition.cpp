#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/photo/photo.hpp>
#include <opencv2/photo.hpp>
#include "polygon.h"

#include "expandedpolygon.h"
#include "imagerecognition.h"
//#include "ui_imagerecognition.h"

void ImageRecognition::run(cv::Mat raw_flame_image, cv::Mat raw_pieces_image)
{
    raw_pieces_pic = cv::Mat(raw_pieces_image,cv::Rect(250,0,1450,1080));

    //前処理
    cv::Mat flame_image = preprocessingFlame(raw_flame_image);
    std::vector<cv::Mat> pieces_images = preprocessingPieces(raw_pieces_image);

    std::vector<cv::Mat> images;
    images.push_back(flame_image);
    for(cv::Mat& piece : pieces_images) images.push_back(piece);

    //線分検出
    std::vector<std::vector<cv::Vec4f>> pieces_lines = LineDetection(images);

    //ベクター化
    std::vector<procon::ExpandedPolygon> polygons = Vectored(pieces_lines);
}

void ImageRecognition::threshold(cv::Mat& image)
{
    //resize
    image = cv::Mat(image,cv::Rect(250,0,1450,1080));

    //色抽出 H:0-180/180, S:76-255/255, B:76-153/255
    colorExtraction(&image, &image, CV_BGR2HSV, 0, 180, 76, 255, 76, 153);

    //グレースケールに変換
    cvtColor(image,image,CV_RGB2GRAY);

    //二値化
    cv::threshold(image, image, 0, 255, cv::THRESH_BINARY_INV);
}

cv::Mat ImageRecognition::preprocessingFlame(cv::Mat image)
{
    threshold(image);
    int rows = image.rows;
    int cols = image.cols;

    //ピース内に混じっている白い穴を削除
    cv::Mat hole_label;
    cv::Mat hole_label_stats;
    cv::Mat buf;
    int hole_num = cv::connectedComponentsWithStats(image, hole_label, hole_label_stats, buf, 4);
    std::vector<int> labels_area;
    for(int i=2;i<hole_num;++i){
        labels_area.push_back(hole_label_stats.ptr<int>(i)[cv::ConnectedComponentsTypes::CC_STAT_AREA]);
    }
    int max_val=0;
    int max_num=0;
    for(int i=0;i<(int)labels_area.size();++i){
        if(labels_area[i] > max_val){
            max_val = labels_area[i];
            max_num = i + 2;
        }
    }
    for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++) image.at<unsigned char>(y,x) = (hole_label.at<int>(y,x) == 1 || hole_label.at<int>(y,x) == max_num) ? 255 : 0;

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
    for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++){
        for(int i=0;i<label_num;++i){
            images[i].at<unsigned char>(y,x) = 255;
        }
    }
    int n;
    for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++)
    {
        n = piece_label->at<int>(y,x);
        if(n!=0){
            images[n-1].at<unsigned char>(y,x) = 0;
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
    return std::move(result_images[0]);
}

std::vector<cv::Mat> ImageRecognition::preprocessingPieces(cv::Mat image)
{
    threshold(image);
    int rows = image.rows;
    int cols = image.cols;

    //ピース内に混じっている白い穴を削除
    cv::Mat hole_label;
    cv::connectedComponents(image, hole_label,4);
    for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++) image.at<unsigned char>(y,x) = hole_label.at<int>(y,x) == 1  ? 255 : 0;

    //二値化
    cv::threshold(image, image, 0, 255, cv::THRESH_BINARY_INV);

    //ピースを一つ一つに分ける

    //get blackzone label
    cv::Mat blackzone_label;
    cv::Mat blackzone_stats;
    cv::Mat blackzone_centroids;
    int blackzone_num = cv::connectedComponentsWithStats(image, blackzone_label, blackzone_stats, blackzone_centroids, 4);

    //delete small noise from image
    const int NOIZE_SIZE = 100;
    std::vector<bool> isErase;
    for(int i=0;i<blackzone_num;++i){
        if(i==0){
            //first label is white zone.
            isErase.push_back(false);
        }else{
            isErase.push_back(blackzone_stats.ptr<int>(i)[cv::ConnectedComponentsTypes::CC_STAT_AREA] < NOIZE_SIZE ? true : false);
        }
    }
    for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++)
    {
        if(isErase.at(blackzone_label.at<int>(y,x))){
            image.at<unsigned char>(y,x) = 0;
        }
    }

    //get pieces label
    cv::Mat pieces_label;
    cv::Mat pieces_stats;
    cv::Mat pieces_centroids;
    int piece_num = cv::connectedComponentsWithStats(image, pieces_label, pieces_stats, pieces_centroids, 4);
    piece_num--; //white zone

    //make random color
    std::random_device rd;
    std::uniform_real_distribution<double> rand(0,255);
    for(int i=0;i<piece_num;++i){
        raw_random_colors.push_back(cv::Vec3b(rand(rd),rand(rd),rand(rd)));
    }

    //reset images
    std::vector<cv::Mat> images;
    for(int i=0;i<piece_num;++i) images.push_back(cv::Mat(rows,cols,CV_8UC1));
    for(int i=0;i<piece_num;++i){
        for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++){
            images.at(i).at<unsigned char>(y,x) = 255;
        }
    }

    //sepalate image, and calc minmax, and make color image
    struct minmax2D{
        int minX = 10000;
        int maxX = 0;
        int minY = 10000;
        int maxY = 0;
    };
    std::vector<struct minmax2D> minmaxs(piece_num);
    cv::Mat color_image(rows,cols,CV_8UC3);
    for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++)
    {
        int n = pieces_label.at<int>(y,x);
        if(n!=0){
            //separate image
            images[n-1].at<unsigned char>(y,x) = 0;
            //calc minmax
            if(minmaxs[n-1].minX > x) minmaxs[n-1].minX = x;
            if(minmaxs[n-1].maxX < x) minmaxs[n-1].maxX = x;
            if(minmaxs[n-1].minY > y) minmaxs[n-1].minY = y;
            if(minmaxs[n-1].maxY < y) minmaxs[n-1].maxY = y;
        }
        //make color image
        color_image.at<cv::Vec3b>(y,x) = (n==0) ? cv::Vec3b(255,255,255) : raw_random_colors[n-1];
    }
    raw_colored_pic = color_image;

    //triming, and get piece raw pos
    std::vector<cv::Mat> result_images;
    int count=0;
    for(auto &im: images){
        result_images.push_back(cv::Mat(im,cv::Rect(minmaxs[count].minX - 5 < 0 ? 0 : minmaxs[count].minX - 5,
                                                    minmaxs[count].minY - 5 < 0 ? 0 : minmaxs[count].minY - 5,
                                                    minmaxs[count].maxX + 5 > cols ? cols-minmaxs[count].minX : minmaxs[count].maxX-minmaxs[count].minX + 10,
                                                    minmaxs[count].maxY + 5 > rows ? rows-minmaxs[count].minY : minmaxs[count].maxY-minmaxs[count].minY + 10
                                                    )));
        raw_pieces_pos.push_back({minmaxs[count].minX+((minmaxs[count].maxX-minmaxs[count].minX)/2),
                                  minmaxs[count].minY+((minmaxs[count].maxY-minmaxs[count].minY)/2)});
        count++;
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

std::vector<procon::ExpandedPolygon> ImageRecognition::Vectored(std::vector<std::vector<cv::Vec4f>> const& pieces_lines)
{
    std::vector<procon::ExpandedPolygon> polygon;
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
