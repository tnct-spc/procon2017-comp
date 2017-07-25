#ifndef NEOANSWERBOARD_H
#define NEOANSWERBOARD_H

#include <QWidget>
#include <opencv2/core/core.hpp>
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
    QRgb NeosetRandomColors();

private:
    Ui::NeoAnswerBoard *ui;
    int grid_size;
<<<<<<< HEAD
=======
    int top_bottom_margin;
    int left_right_margin;
    int top_margin;
    procon::Field field;
>>>>>>> a7fe985b2eb145cc480c994eefb7db0f21ff7a81

protected:
    void beforePolygon();
    void paintEvent(QPaintEvent *event);
};

#endif // NEOANSWERBOARD_H
