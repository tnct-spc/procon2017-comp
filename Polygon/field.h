#ifndef FIELD_H
#define FIELD_H

#include "expandedpolygon.h"

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
    std::vector<procon::ExpandedPolygon> elementary_inverse_pieces;

    //ピースが置かれているか保存する変数
    std::array<bool,50> isPlaced;

    //ピースの最小辺最小角
    double min_angle;
    double min_side;

public:
    //constructor
    Field();

    //setter
    void setFlame(procon::ExpandedPolygon const& flame);
    void setPiece(polygon_t piece);
    void setPiece(procon::ExpandedPolygon piece);
    void setPiece(procon::ExpandedPolygon piece,double x, double y);
    void setPiece(procon::ExpandedPolygon piece,int n,double x = 0,double y = 0);
    void setElementaryFlame(procon::ExpandedPolygon const& flame);
    void setElementaryPieces(std::vector<procon::ExpandedPolygon> const& pieces);
    void setIsPlaced(std::array<bool,50> const& IsPlaced);
    void setIsPlaced(int const& piece_id);

    //getter
    std::vector<procon::ExpandedPolygon> const& getPieces() const;
    procon::ExpandedPolygon const& getPiece(int const& n) const;
    procon::ExpandedPolygon const& getFlame() const;
    procon::ExpandedPolygon const& getElementaryFlame() const;
    std::vector<procon::ExpandedPolygon> const& getElementaryPieces() const;
    std::vector<procon::ExpandedPolygon> const& getElementaryInversePieces() const;
    std::array<bool,50> const& getIsPlaced() const;
    int getFieldScore() const;
    int getPiecesSize() const;
    double getMinAngle() const;
    double getMinSide() const;

    //任意の位置のピースを消去
    void removePiece(int n);

    //置けるかチェック
    bool isPuttable(procon::ExpandedPolygon polygon);

    //コンソール出力
    void printFlame();
    void printPiece();

    //translate polygon
    static polygon_t translatePolygon(polygon_t polygon, double x, double y);

    //最小辺最小角を計算する
    void calcMinAngleSide();
};
}
#endif // FIELD_H
