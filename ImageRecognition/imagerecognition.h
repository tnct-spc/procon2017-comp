#ifndef IMAGERECOGNITION_H
#define IMAGERECOGNITION_H

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
    procon::Field run(std::string const& path);

private:
    std::vector<cv::Mat> Preprocessing(std::string const& path);
    std::vector<std::vector<cv::Vec4f>> LineDetection(std::vector<cv::Mat> const& images);
    std::vector<polygon_t> Vectored(std::vector<std::vector<cv::Vec4f>> const& lines);
    procon::Field makeField(std::vector<polygon_t> pieces);
    void colorExtraction(cv::Mat* src, cv::Mat* dst,
        int code,
        int ch1Lower, int ch1Upper,
        int ch2Lower, int ch2Upper,
        int ch3Lower, int ch3Upper
        );
    std::vector<SinglePolygonDisplay*> disp;
};

#endif // IMAGERECOGNITION_H
