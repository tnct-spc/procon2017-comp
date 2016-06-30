#ifndef POLYGONEXPANSION_H
#define POLYGONEXPANSION_H
#include <iostream>
#include <boost/geometry.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <vector>
#include <cmath>
namespace bg = boost::geometry;
typedef bg::model::d2::point_xy<double> point_t;
typedef bg::model::polygon<point_t> polygon_t;

class PolygonExpansion
{
    polygon_t polygon;
    int n; //頂点の数
    //各辺の長さ
    //sideLength[0]は0つ目の点と1つ目の点を結ぶ辺の長さ
    std::vector<double> sideLength;
    //各頂点の角度(ただし内角とは限らない)
    //arctanの値域的に，内角がpi[rad]を超えると代わりに外角の角度がでる？
    std::vector<double> sideAngle;
    //calc関数群はPolygonExpansionのポリゴンを更新したときに用いる
    //頂点の数の計算
    void calcN();
    //各辺の長さの計算
    void calcSideLength();
    //各頂点の角度の計算
    void calcSideAngle();
public:
    PolygonExpansion();
    /*getterとsetter*/
    polygon_t getPolygon();
    const std::vector<double> getSideLength();
    const std::vector<double> getSideAngle();
    void setPolygon(const polygon_t &pol);
};

#endif // POLYGONEXPANSION_H
