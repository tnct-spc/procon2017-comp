#ifndef NEOANSWERBOARD_H
#define NEOANSWERBOARD_H

#include <QWidget>
#include <field.h>
#include <iostream>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/strategies/transform.hpp>
#include <boost/geometry/strategies/transform/matrix_transformers.hpp>
#include <boost/geometry/algorithms/disjoint.hpp>
#include "../Polygon/neoexpandedpolygon.h"
#include <vector>
#include "field.h"
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

private:
    Ui::NeoAnswerBoard *ui;
    polygon_i getPolygon();
    std::vector<polygon_i> piecepolygon;
    polygon_i framepolygon;
    QPointF getPosition(point_i point);
    int grid_size;
    int top_bottom_margin;
    int left_right_margin;
    int top_margin;
    procon::Field field;

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // NEOANSWERBOARD_H
