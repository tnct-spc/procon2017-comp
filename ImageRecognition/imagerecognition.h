#ifndef IMAGERECOGNITION_H
#define IMAGERECOGNITION_H

#include "imagerecognition_global.h"
#include "polygonexpansion.h"
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class IMAGERECOGNITIONSHARED_EXPORT ImageRecognition
{

public:
    void run();

private:
    cv::Mat preprocessingFlame(std::string const& path);
    std::vector<cv::Mat> preprocessingPieces(std::string const& path);
    std::vector<std::vector<cv::Vec4f>> LineDetection(std::vector<cv::Mat> const& images);
    std::vector<PolygonExpansion> Vectored(std::vector<std::vector<cv::Vec4f>> const& lines);
    void threshold(cv::Mat& image);
    void colorExtraction(cv::Mat* src, cv::Mat* dst,
        int code,
        int ch1Lower, int ch1Upper,
        int ch2Lower, int ch2Upper,
        int ch3Lower, int ch3Upper
        );
};

#endif // IMAGERECOGNITION_H
