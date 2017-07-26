#ifndef BEFOREBOARD_H
#define BEFOREBOARD_H

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
class BeforeBoard;
}

class BeforeBoard : public QWidget
{
    Q_OBJECT

public:
    explicit BeforeBoard(QWidget *parent = 0);
    ~BeforeBoard();

private:
    Ui::BeforeBoard *ui;

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // BEFOREBOARD_H
