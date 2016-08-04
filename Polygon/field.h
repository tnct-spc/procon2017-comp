#ifndef FIELD_H
#define FIELD_H

#include "expandedpolygon.h"
#include <iostream>
#include <vector>
#include <boost/geometry.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/algorithms/disjoint.hpp>

namespace procon {
class Field
{
private:
    procon::ExpandedPolygon field_flame;
    std::vector<procon::ExpandedPolygon> field_pieces;
public:
    Field();
    //setter
    void setFlame(const procon::ExpandedPolygon &flame);
    void setPiece(procon::ExpandedPolygon piece,double x=0, double y=0);
    void setPiece(procon::ExpandedPolygon piece,int n,double x=0,double y=0);
    //getter
    std::vector<procon::ExpandedPolygon> getPieces();
    procon::ExpandedPolygon getPiece(const int &n);
    procon::ExpandedPolygon getFlame();
    int getPiecesSize();
    //任意の位置のピースを消去
    void removePiece(int n);
    //置けるかチェック
    bool isPuttable(procon::ExpandedPolygon polygon);
    //コンソール出力
    void printFlame();
    void printPiece();
};
}
#endif // FIELD_H
