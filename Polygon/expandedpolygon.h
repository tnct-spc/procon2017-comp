#ifndef EXPANDEDPOLYGON_H
#define EXPANDEDPOLYGON_H

#include "fit.h"

namespace bg = boost::geometry;
using point_t = bg::model::d2::point_xy<double>;
using ring_t = bg::model::ring<point_t>;
using polygon_t = bg::model::polygon<point_t,true,true,std::vector,std::vector,std::allocator,std::allocator>;

namespace procon {

class ExpandedPolygon
{
    //メンバ
    int size;
    int id;
    std::vector<int> multi_ids;
    std::vector<double> side_length;
    std::vector<double> side_angle;
    std::vector<double> side_slope;

    int inners_size;
    std::vector<std::vector<double>> inners_side_length;
    std::vector<std::vector<double>> inners_side_angle;
    std::vector<std::vector<double>> inners_side_slope;

    polygon_t polygon;

    std::vector<procon::ExpandedPolygon> jointed_pieces;

    //flag
    bool calcSize_flag = false;

protected:
    //calc
    void calcSize();
    void calcSideLength();
    void calcSideAngle();
    void calcSideSlope();


public:
    // Public Member

    //constructor
    ExpandedPolygon(int id_ = -1);
    ExpandedPolygon(std::vector<int> multi_ids_);
    ExpandedPolygon(ExpandedPolygon const& p);

    //getter
    int getSize() const;
    int getInnerSize() const;
    std::vector<double> const& getSideLength() const;
    std::vector<double> const& getSideAngle() const;
    std::vector<double> const& getSideSlope() const;
    std::vector<std::vector<double>> const& getInnersSideLength() const;
    std::vector<std::vector<double>> const& getInnersSideAngle() const;
    std::vector<std::vector<double>> const& getInnersSideSlope() const;
    polygon_t const& getPolygon() const;
    int getId() const;
    std::vector<int> getMultiIds() const;
    std::string makeMultiIdString() const;
    std::vector<procon::ExpandedPolygon> const& getJointedPieces() const;

    //setter
    void setMultiIds(std::vector<int> multi_ids_);
    void resetPolygonForce(polygon_t const & p);
    void pushNewJointedPolygon(polygon_t const & new_frame, procon::ExpandedPolygon const& jointed_polygon);
    void replaceJointedPieces(std::vector<procon::ExpandedPolygon> pieces);

    //operator
    ExpandedPolygon operator = (ExpandedPolygon const& p);

    //calcAll
    void updatePolygon(bool calc = false);

    void inversePolygon();
    void rotatePolygon(double degree);
    void translatePolygon(double x,double y);

    void setPolygonAngle(double degree);
    void setPolygonPosition(double x,double y);

    void sortJointedPieces();

    double difference_of_default_degree = 0;

    double centerx = 0;
    double centery = 0;

    bool is_inverse = false;
};

}
#endif // EXPANDEDPOLYGON_H
