#ifndef FIELD_H
#define FIELD_H
#include <iostream>
#include <boost/geometry.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <vector>
namespace bg = boost::geometry;
typedef bg::model::d2::point_xy<double> point_t;
typedef bg::model::polygon<point_t> polygon_t;


class Field
{
    polygon_t fieldFlame;
    std::vector<polygon_t> fieldPiece;
    std::vector<polygon_t>::iterator  fieldPieceIteratorEnd; //fieldPieceの末尾を示すirerator
public:
    Field();
    void setFlame(const polygon_t &flame); //フレーム多角形をセット
    void pushPiece(const polygon_t &piece); //ピース多角形をセット（末尾）
    polygon_t popPiece(); //末尾のピース多角形をpop
    polygon_t getPiece(const int &n);
    polygon_t getFlame();
    void printFlame();
    void printPiece();
};

#endif // FIELD_H
