#ifndef NEOFIELD_H
#define NEOFIELD_H

#include "neoexpandedpolygon.h"

namespace procon {
class NeoField
{
private:
    std::bitset<50> piece_id;
private:
    //フィールド上のピース&フレーム
    procon::NeoExpandedPolygon field_frame;
    std::vector<procon::NeoExpandedPolygon> field_pieces;

    //素のピース&フレーム
    procon::NeoExpandedPolygon elementary_frame;
    std::vector<procon::NeoExpandedPolygon> elementary_pieces;
    std::vector<procon::NeoExpandedPolygon> elementary_inverse_pieces;

    //ピースが置かれているか保存する変数
    std::array<bool,50> isPlaced;

    //ピースの最小辺最小角
    double min_angle;
    double min_side;

    //フィールドの累計評価値
    double total_evaluation = 0;

public:
    bool operator || (NeoField & field_id);

    //constructor
    NeoField();

    //setter
    void setFrame(procon::NeoExpandedPolygon const& frame);
    void setPiece(polygon_i piece);
    void setPiece(procon::NeoExpandedPolygon piece);
    void setPiece(procon::NeoExpandedPolygon piece,double x, double y);
    void setPiece(procon::NeoExpandedPolygon piece,int n,double x = 0,double y = 0);
    void setElementaryFrame(procon::NeoExpandedPolygon const& frame);
    void setElementaryPieces(std::vector<procon::NeoExpandedPolygon> const& pieces);
    void setIsPlaced(std::array<bool,50> const& IsPlaced);
    void setIsPlaced(int const& piece_id);
    void sumTotalEvaluation(double const& eva);

    //getter
    std::vector<procon::NeoExpandedPolygon> const& getPieces() const;
    procon::NeoExpandedPolygon const& getPiece(int const& n) const;
    procon::NeoExpandedPolygon const& getFrame() const;
    procon::NeoExpandedPolygon const& getElementaryFrame() const;
    std::vector<procon::NeoExpandedPolygon> const& getElementaryPieces() const;
    std::vector<procon::NeoExpandedPolygon> const& getElementaryInversePieces() const;
    std::array<bool,50> const& getIsPlaced() const;
    int getFieldScore() const;
    int getPiecesSize() const;
    double getMinAngle() const;
    double getMinSide() const;
    std::bitset<50> const& getPieceID() const;
    std::vector<double> const& getSlopeID() const;
    std::vector<point_i> const& getCoordID() const;
    double getTotalEvaluation() const;

    //任意の位置のピースを消去
    void removePiece(int n);

    //置けるかチェック
    bool isPuttable(procon::NeoExpandedPolygon polygon);

    //コンソール出力
    void printFrame();
    void printPiece();

    //translate polygon
    static polygon_i translatePolygon(polygon_i polygon, double x, double y);

    //最小辺最小角を計算する
    void calcMinAngleSide();

    //フィールドのIDを計算する
    void calcFieldID();
};
}
#endif // NEOFIELD_H