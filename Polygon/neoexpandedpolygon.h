#ifndef NEOEXPANDEDPOLYGON_H
#define NEOEXPANDEDPOLYGON_H

#include "fit.h"
#include <iostream>
#include <vector>

namespace procon {

class point {

    // グリッド点のx,yを保存
    int x;
    int y;

public:
    // constractor
    point(){
        x = 0;
        y = 0;
    }

    point(int x_, int y_) {
        x = x_;
        y = y_;
    }
};

class NeoExpandedPolygon
{
    //メンバ
    int size;
    int id;
    std::vector<point> points;
    std::vector<point> inner_points;

    // std::vector<int> multi_ids;
    std::vector<double> side_length;
    std::vector<double> side_angle;
    std::vector<double> side_slope;

    int inners_size;
    std::vector<std::vector<double>> inners_side_length;
    std::vector<std::vector<double>> inners_side_angle;
    std::vector<std::vector<double>> inners_side_slope;

    //flag
    bool calcSize_flag = false;

public:
    //constructor
    NeoExpandedPolygon();
    NeoExpandedPolygon(std::vector<point>);
    NeoExpandedPolygon(int id_ = -1);
    // NeoExpandedPolygon(std::vector<int> multi_ids_);
    NeoExpandedPolygon(NeoExpandedPolygon const& p);

    //getter
    int getSize() const;
    int getInnerSize() const;
    // std::vector<double> const& getSideLength() const;
    // std::vector<double> const& getSideAngle() const;
    // std::vector<double> const& getSideSlope() const;
    // std::vector<std::vector<double>> const& getInnersSideLength() const;
    // std::vector<std::vector<double>> const& getInnersSideAngle() const;
    // std::vector<std::vector<double>> const& getInnersSideSlope() const;
    int getId() const;
    // std::vector<int> getMultiIds() const;
    // std::string makeMultiIdString() const;
    // std::vector<procon::ExpandedPolygon> const& getJointedPieces() const;

    //setter
    // void setMultiIds(std::vector<int> multi_ids_);
    // void resetPolygonForce(polygon_t const & p);
    // void pushNewJointedPolygon(polygon_t const & new_frame, procon::ExpandedPolygon const& jointed_polygon);
    // void replaceJointedPieces(std::vector<procon::ExpandedPolygon> pieces);

    //operator
    // ExpandedPolygon operator = (ExpandedPolygon const& p);

    //calcAll
    // void updatePolygon(bool calc = false);

    void calcSize();
    // void inversePolygon();
    // void rotatePolygon(double degree);
    // void translatePolygon(double x,double y);

    // void setPolygonAngle(double degree);
    // void setPolygonPosition(double x,double y);

    // void sortJointedPieces();

    // double difference_of_default_degree = 0;

    // double centerx = 0;
    // double centery = 0;

    // bool is_inverse = false;
};
}

#endif // NEOEXPANDEDPOLYGON_H
