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


    static polygon_t inversePolygon(polygon_t polygon);


    static polygon_t rotatePolygon(polygon_t polygon,double degree);
};

}
#endif // EXPANDEDPOLYGON_H
