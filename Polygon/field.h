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
    //フィールド上のピース&フレーム
    procon::ExpandedPolygon field_flame;
    std::vector<procon::ExpandedPolygon> field_pieces;

    //素のピース&フレーム
    procon::ExpandedPolygon elementary_flame;
    std::vector<procon::ExpandedPolygon> elementary_pieces;
public:
    //constructor
    Field();

    //setter
    void setFlame(procon::ExpandedPolygon const& flame);
    void setPiece(procon::ExpandedPolygon piece,double x=0, double y=0);
    void setPiece(procon::ExpandedPolygon piece,int n,double x=0,double y=0);
    void setElementaryFlame(procon::ExpandedPolygon const& flame);
    void setElementaryPieces(std::vector<procon::ExpandedPolygon> const& pieces);

    //getter
    std::vector<procon::ExpandedPolygon> const& getPieces() const;
    procon::ExpandedPolygon const& getPiece(int const& n) const;
    procon::ExpandedPolygon const& getFlame() const;
    procon::ExpandedPolygon const& getElementaryFlame() const;
    std::vector<procon::ExpandedPolygon> const& getElementaryPieces() const;
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
