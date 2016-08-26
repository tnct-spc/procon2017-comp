#ifndef EXPANDEDPOLYGON_H
#define EXPANDEDPOLYGON_H
#include <iostream>
#include <exception>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/algorithms/distance.hpp>
#include <boost/geometry/algorithms/transform.hpp>
#include <boost/assign/list_of.hpp>
namespace bg = boost::geometry;
using point_t = bg::model::d2::point_xy<double>;
using ring_t = bg::model::ring<point_t>;
using polygon_t = bg::model::polygon<point_t,true,true,std::vector,std::vector,std::allocator,std::allocator>;

namespace procon {

class ExpandedPolygon
{
    //メンバ
    int size;
    std::vector<double> side_length;
    std::vector<double> side_angle;
    polygon_t polygon;

    double first_degree = 0;

    double centerx = 1;
    double centery = 1;

    //calc
    //calcSideSize -> calcSideLength -> calcSideAngle
    void calcSize();
    void calcSideLength();
    void calcSideAngle();

public:
    //constructor
    ExpandedPolygon();
    ExpandedPolygon(ExpandedPolygon const& p);

    //getter
    int getSize() const;
    std::vector<double> const& getSideLength() const;
    std::vector<double> const& getSideAngle() const;
    polygon_t const& getPolygon() const;

    //setter
    void setPolygon(polygon_t const & p);

    //operator
    ExpandedPolygon operator =  (ExpandedPolygon const& p);

    //calcAll
    //***ポリゴン変更後必ず実行のこと***
    void updatePolygon();

    void inversePolygon();
    void rotatePolygon(double degree);
    void translatePolygon(double x,double y);
    
    void setPolygonAngle(double degree);
    void setPolygonPosition(double x,double y);


};

}
#endif // EXPANDEDPOLYGON_H
