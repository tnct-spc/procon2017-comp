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
    cv::Mat Preprocessing(std::string const& path);
    std::vector<cv::Vec4f> LineDetection(cv::Mat const& image);
    std::vector<PolygonExpansion> Vectored(std::vector<cv::Vec4f> const& lines);
};

#endif // IMAGERECOGNITION_H
