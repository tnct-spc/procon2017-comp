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
public:
    Field();
    void setFlame(const polygon_t &flame); //フレーム多角形をセット
    void setPiece(const polygon_t &piece,const int &n); //ピース多角形をセット(任意)
    void pushPiece(const polygon_t &piece); //ピース多角形をセット（末尾）
    polygon_t popPiece(); //末尾のピース多角形をpop
    const polygon_t &getPiece(const int &n) const;
    const polygon_t &getFlame() const;
    int pieceSize();
    void printFlame(); //コンソール
    void printPiece(); //コンソール
};

#endif // FIELD_H
