#ifndef FIELD_H
#define FIELD_H
#include <iostream>
#include <boost/geometry.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <vector>
#include "polygonexpansion.h"
namespace bg = boost::geometry;
typedef bg::model::d2::point_xy<double> point_t;
typedef bg::model::polygon<point_t> polygon_t;


class Field
{
    PolygonExpansion fieldFlame;
    std::vector<PolygonExpansion> fieldPiece;
public:
    Field();
    //setterとgetter
    void setFlame(const PolygonExpansion &flame); 
    void setPiece(const PolygonExpansion &piece,const int &n);
    void pushPiece(const PolygonExpansion &piece);
    PolygonExpansion popPiece();
    PolygonExpansion getPiece(const int &n) ;
    PolygonExpansion getFlame() ;
    //以下はsetterとgetterの後方互換用 *↑とオーバーロードできない(getterの引数が同じ)なので気をつけて*
    /*
    void setFlame(const polygon_t &flame);
    void setPiece(const polygon_t &piece,const int &n);
    void pushPiece(const polygon_t &piece);
    polygon_t popPiece();
    polygon_t getPiece(const int &n) ;
    polygon_t getFlame();
    */
    //*ここまで*
    //fieldPieceにセットされているピースの数
    int pieceSize();
    //コンソール出力
    void printFlame();
    void printPiece();
};

#endif // FIELD_H
