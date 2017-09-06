#ifndef NEOEXPANDEDPOLYGON_H
#define NEOEXPANDEDPOLYGON_H

#include "fit.h"
#include "precompile.h"

namespace bg = boost::geometry;
using point_i = bg::model::d2::point_xy<int>;
//using ring_t = bg::model::ring<point_i>;
using polygon_i = bg::model::polygon<point_i,true,true,std::vector,std::vector,std::allocator,std::allocator>;

namespace procon {

class NeoExpandedPolygon
{
private:
    //メンバ
    int size;
    int id;
    std::vector<int> multi_ids;
    std::vector<double> side_length;
    std::vector<double> side_angle;
    std::vector<double> side_slope;
    //int inners_size;
    //std::vector<std::vector<double>> inners_side_length;
    //std::vector<std::vector<double>> inners_side_angle;
    //std::vector<std::vector<double>> inners_side_slope;
    polygon_i polygon;

    std::vector<procon::NeoExpandedPolygon> jointed_pieces;

    //flag
    bool calcSize_flag = false;

protected:
    //calc
    void calcSize();
    void calcSideLength();
    void calcSideAngle();
    void calcSideSlope();


public:
    //constructor
    NeoExpandedPolygon(int id_ = -1);
    NeoExpandedPolygon(std::vector<int> multi_ids_);
    NeoExpandedPolygon(NeoExpandedPolygon const& p);

    //getter
    int getSize() const;
    //int getInnerSize() const;
    std::vector<double> const& getSideLength() const;
    std::vector<double> const& getSideAngle() const;
    std::vector<double> const& getSideAngle_degree() const;
    std::vector<double> const& getSideSlope() const;
    //std::vector<std::vector<double>> const& getInnersSideLength() const;
    //std::vector<std::vector<double>> const& getInnersSideAngle() const;
    //std::vector<std::vector<double>> const& getInnersSideSlope() const;
    polygon_i const& getPolygon() const;
    int getId() const;
    std::vector<int> getMultiIds() const;
    std::string makeMultiIdString() const;
    std::vector<procon::NeoExpandedPolygon> const& getJointedPieces() const;

    //setter
    void setMultiIds(std::vector<int> multi_ids_);
    void resetPolygonForce(polygon_i const & p);
    void pushNewJointedPolygon(polygon_i const & new_frame, procon::NeoExpandedPolygon const& jointed_polygon);
    void replaceJointedPieces(std::vector<procon::NeoExpandedPolygon> pieces);

    //operator
    NeoExpandedPolygon operator = (NeoExpandedPolygon const& p);

    //calcAll
    void updatePolygon(bool calc = false);

    void inversePolygon();
    //void rotatePolygon(double degree);
    void translatePolygon(int x,int y);

    //void setPolygonAngle(double degree);
    void setPolygonPosition(int x,int y);

    void sortJointedPieces();

    double difference_of_default_degree = 0;

    int centerx = 0;
    int centery = 0;

    bool is_inverse = false;
};

}

#endif // NEOEXPANDEDPOLYGON_H
