#ifndef FIELD_H
#define FIELD_H

#include "expandedpolygon.h"

namespace procon {
class Field
{
private:
    std::bitset<50> piece_id;
private:
    //フィールド上のピース&フレーム
    procon::ExpandedPolygon field_frame;
    std::vector<procon::ExpandedPolygon> field_pieces;

    //素のピース&フレーム
    procon::ExpandedPolygon elementary_frame;
    std::vector<procon::ExpandedPolygon> elementary_pieces;
    std::vector<procon::ExpandedPolygon> elementary_inverse_pieces;

    //ピースが置かれているか保存する変数
    std::array<bool,50> isPlaced;

    //ピースの最小辺最小角
    double min_angle;
    double min_side;

public:
    // Public Member
    double evaluation;

    bool operator || (Field & field_id);

    //constructor
    Field();

    //setter
    void setFrame(procon::ExpandedPolygon const& frame);
    void setPiece(polygon_t piece);
    void setPiece(procon::ExpandedPolygon piece);
    void setPiece(procon::ExpandedPolygon piece,double x, double y);
    void setPiece(procon::ExpandedPolygon piece,int n,double x = 0,double y = 0);
    void setElementaryFrame(procon::ExpandedPolygon const& frame);
    void setElementaryPieces(std::vector<procon::ExpandedPolygon> const& pieces);
    void setIsPlaced(std::array<bool,50> const& IsPlaced);
    void setIsPlaced(int const& piece_id);

    //getter
    std::vector<procon::ExpandedPolygon> const& getPieces() const;
    procon::ExpandedPolygon const& getPiece(int const& n) const;
    procon::ExpandedPolygon const& getFrame() const;
    procon::ExpandedPolygon const& getElementaryFrame() const;
    std::vector<procon::ExpandedPolygon> const& getElementaryPieces() const;
    std::vector<procon::ExpandedPolygon> const& getElementaryInversePieces() const;
    std::array<bool,50> const& getIsPlaced() const;
    int getFieldScore() const;
    int getPiecesSize() const;
    double getMinAngle() const;
    double getMinSide() const;
    std::bitset<50> const& getPieceID() const;
    std::vector<double> const& getSlopeID() const;
    std::vector<point_t> const& getCoordID() const;

    //任意の位置のピースを消去
    void removePiece(int n);

    //置けるかチェック
    bool isPuttable(procon::ExpandedPolygon polygon);

    //コンソール出力
    void printFrame();
    void printPiece();

    //translate polygon
    static polygon_t translatePolygon(polygon_t polygon, double x, double y);

    //最小辺最小角を計算する
    void calcMinAngleSide();

    //フィールドのIDを計算する
    void calcFieldID();
};
}
#endif // FIELD_H
