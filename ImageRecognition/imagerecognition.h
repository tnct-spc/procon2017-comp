#ifndef IMAGERECOGNITION_H
#define IMAGERECOGNITION_H

#include <random>

#include "expandedpolygon.h"
#include "imagerecognition_global.h"
#include "expandedpolygon.h"
#include "singlepolygondisplay.h"
#include "field.h"
#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/photo/photo.hpp>
#include <opencv2/photo.hpp>

class IMAGERECOGNITIONSHARED_EXPORT ImageRecognition
{

public:
    procon::Field run(cv::Mat raw_flame_image, cv::Mat raw_pieces_image);

    inline cv::Mat getRawPiecesPic(){
        return std::move(raw_colored_pic);
    }

    inline std::vector<cv::Point> getRawPiecesPos(){
        return std::move(raw_pieces_pos);
    }

    inline std::vector<cv::Vec3b> getRawRandomColors(){
        return std::move(raw_random_colors);
    }

private:
    cv::Mat preprocessingFlame(cv::Mat image);
    std::vector<cv::Mat> preprocessingPieces(cv::Mat image);
    std::vector<std::vector<cv::Vec4f>> LineDetection(std::vector<cv::Mat> const& images);
    std::vector<polygon_t> Vectored(std::vector<std::vector<cv::Vec4f>> const& lines);
    procon::Field makeField(std::vector<polygon_t> pieces);
    void threshold(cv::Mat& image);
    void colorExtraction(cv::Mat* src, cv::Mat* dst,
        int code,
        int ch1Lower, int ch1Upper,
        int ch2Lower, int ch2Upper,
        int ch3Lower, int ch3Upper
        );
    std::vector<SinglePolygonDisplay*> disp;
    cv::Mat raw_pieces_pic;
    cv::Mat raw_colored_pic;
    std::vector<cv::Point> raw_pieces_pos;
    std::vector<cv::Vec3b> raw_random_colors;
    double scale;
};

#endif // IMAGERECOGNITION_H
