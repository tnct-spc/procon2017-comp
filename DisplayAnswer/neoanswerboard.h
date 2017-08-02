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
#include <QList>
#include "neofield.h"
#include "neoexpandedpolygon.h"
#include <math.h>

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
    void setDockMode(bool inp);
private:
    int frame_margin;
    Ui::NeoAnswerBoard *ui;
    QPointF getPiecePosition(point_i point);
    void setField();
    QPointF getPosition(point_i point);
    QVector<QString> list;
    void firstField();
    int left_right_margin;
    int grid_size;
    int top_bottom_margin;
    int down_up_y;
    procon::NeoField field;

    // Only field mode
    bool single_mode = false;

    //make id_list
    std::vector<polygon_i> polygon_list;

    //draw processingLine
    point_i center;
    int point_id = -1;
    int red_id;
    int blue_id;
    bool selecter;//true = left, false = right
    bool paintif;

protected:
    void beforePolygon();
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // NEOANSWERBOARD_H
