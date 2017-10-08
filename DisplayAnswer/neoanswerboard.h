#ifndef NEOANSWERBOARD_H
#define NEOANSWERBOARD_H

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/strategies/transform.hpp>
#include <boost/geometry/strategies/transform/matrix_transformers.hpp>
#include <boost/geometry/algorithms/disjoint.hpp>
#include <QWidget>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <QStringList>
#include <QVector>
#include "neofield.h"
#include "expandedpolygon.h"
#include "neoexpandedpolygon.h"
#include "neosinglepolygondisplay.h"
#include <QMessageBox>

namespace Ui {
class NeoAnswerBoard;
}

class NeoAnswerBoard : public QWidget
{
    Q_OBJECT

public:
    explicit NeoAnswerBoard(QWidget *parent = 0);
    ~NeoAnswerBoard();
    void setField(procon::NeoField input_field);
    void setSingleMode(bool inp);
    void setText(std::string text);
    void setScannedPieces(std::vector<procon::ExpandedPolygon> vec);
    void setUp();
    void setProcessingLineMode(bool inp);
    void setIsEmptyColorFill(bool inp);
    void setSelectPieceMode(bool mode);
    void setGoodAnswerSelectMode(bool mode);
    void setDrawVertexMode(bool mode);

    void setShowUnplacedPieces(bool input);

    std::vector<int> getSelectedPieceId();

private:
    int frame_margin;
//    int clickedpiece_id=-1;
    std::vector<int> clicked_piece_id;
    std::vector<std::vector<bool>> clicked_frame_point;
    bool select_frame_point_mode = false;
    bool drawVertexMode;

    Ui::NeoAnswerBoard *ui;
    QPointF getPiecePosition(point_i point);
    QPointF getPiecePosition(point_t point);
    void setField();
    QPointF getPosition(point_i point);
    void firstField();
    int left_right_margin;
    int grid_size;
    int top_bottom_margin;
    int down_up_y;
    std::vector<procon::ExpandedPolygon> scanned_poly;
    std::vector<procon::NeoExpandedPolygon> touches_poly;
    procon::NeoField field;

    // Only field mode
    bool single_mode = false;
    bool singleif = false;
    int piece_size = 1;
    bool allif = true;
    bool processinglinemode = false;
    bool select_piece_mode = false;
 //   bool showunplacedpieces = false;

    //make id_list
    std::vector<polygon_i> polygon_list;

    //draw processingLine
    std::vector<procon::NeoExpandedPolygon> sorted_poly; //画面の右側から順番にソートしたやつ
    point_i center;
    int point_id = 0;
    int blue_id=-1;
    int red_id = 0;
    bool selecter;//true = left, false = right
    bool pre = false;
    bool paintif = false;
    void correctFrame();

    QString output_string;//ここのメンバ変数に入ってる文字列をAnswerBoardの画面下に表示するようにする

    bool isemptycolorfill = false;
    bool is_select_good_asnwer_mode = false;

signals:
    void selectedField(procon::NeoField field);

protected:
    void beforePolygon();
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // NEOANSWERBOARD_H
