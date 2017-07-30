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
#include "neofield.h"
#include "neoexpandedpolygon.h"

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

private:
    Ui::NeoAnswerBoard *ui;
    QPointF getPiecePosition(point_i point);
    void setField();
    QPointF getPosition(point_i point);
    std::vector<cv::Vec3b> colors;
    void setRandomColors(int threshold);
    void firstField();
    int left_right_margin;
    int grid_size;
    int top_bottom_margin;
    int down_up_y;
    procon::NeoField field;
    point_i center;

protected:
    void beforePolygon();
    void paintEvent(QPaintEvent *event);
};

#endif // NEOANSWERBOARD_H
