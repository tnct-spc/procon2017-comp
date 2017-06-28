#ifndef IMAGERECOGNITION_H
#define IMAGERECOGNITION_H

#include "imagerecognition_global.h"

#include "field.h"
#include "expandedpolygon.h"
#include "neoexpandedpolygon.h"
#include "singlepolygondisplay.h"

class IMAGERECOGNITIONSHARED_EXPORT ImageRecognition
{

public:
    procon::Field run(cv::Mat raw_frame_image, cv::Mat raw_pieces_image);

    const cv::Mat& getRawPiecesPic(){
        return raw_colored_pic;
    }

    const std::vector<cv::Point>& getRawPiecesPos(){
        return raw_pieces_pos;
    }

    const std::vector<cv::Vec3b>& getRawRandomColors(){
        return raw_random_colors;
    }

private:
    cv::Mat preprocessingFrame(cv::Mat image);
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
    std::vector<cv::Vec4f> houghLine(cv::Mat src_image);
    cv::Mat HSVDetection(cv::Mat src_image);
    std::vector<cv::Mat> dividePiece(cv::Mat src_image);
    void placeGrid(polygon_t vertex);

    cv::Mat raw_pieces_pic;
    cv::Mat raw_colored_pic;
    std::vector<cv::Point> raw_pieces_pos;
    std::vector<cv::Vec3b> raw_random_colors;
    double scale;
    static constexpr double cutting_allowance = 0.0;
};

#endif // IMAGERECOGNITION_H
