#ifndef IMAGERECOGNITION_H
#define IMAGERECOGNITION_H

#include "imagerecognition_global.h"

#include "field.h"
#include "neofield.h"
#include "expandedpolygon.h"
#include "neoexpandedpolygon.h"
#include "singlepolygondisplay.h"

namespace trans = bg::strategy::transform;
using degree_t = trans::rotate_transformer<boost::geometry::radian,double,2,2>;

class IMAGERECOGNITIONSHARED_EXPORT ImageRecognition
{

public:
    procon::NeoField run(cv::Mat raw_frame_image, cv::Mat raw_pieces_image);
    std::vector<procon::ExpandedPolygon> getPolygonPosition();

    const cv::Mat& getRawPiecesPic(){
        return raw_colored_pic;
    }

    const std::vector<cv::Point>& getRawPiecesPos(){
        return raw_pieces_pos;
    }

    const std::vector<cv::Vec3b>& getRawRandomColors(){
        return raw_random_colors;
    }

    typedef struct {
        int x;
        int y;
        double error;
    } error_t;

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
    cv::Mat HSVDetection(cv::Mat src_image);
    std::vector<cv::Mat> dividePiece(cv::Mat src_image);
    polygon_i placeGrid(polygon_t vertex);
    double getError(std::vector<polygon_i> p);
    procon::NeoField makeNeoField(std::vector<polygon_i> pieces);
    void makeTable();

    cv::Mat raw_pieces_pic;
    cv::Mat raw_colored_pic;
    std::vector<cv::Point> raw_pieces_pos;
    std::vector<cv::Vec3b> raw_random_colors;
    double scale;
    static constexpr double cutting_allowance = 0.0;

    std::vector<int> area;
    int field_num;
    int pieces_num;
    std::vector<procon::ExpandedPolygon> position;
    int id = -1;
    int n = 1;
    double frame_deg;
    std::map<point_i, double> tab;

    std::vector<std::pair<point_i,double>> length_table;
};

#endif // IMAGERECOGNITION_H
