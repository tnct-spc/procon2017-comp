#ifndef BEFORE_NEODISPLAYANSWER_H
#define BEFORE_NEODISPLAYANSWER_H

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

namespace Ui {
class Before_NeoAnswerBoard;
}

class Before_NeoAnswerBoard : public QWidget
{
    Q_OBJECT

public:
    explicit Before_NeoAnswerBoard(QWidget *parent = 0);
    ~Before_NeoAnswerBoard();

private:
    Ui::Before_NeoAnswerBoard *ui;

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // BEFORE_NEODISPLAYANSWER_H
