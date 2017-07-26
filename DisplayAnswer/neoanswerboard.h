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
#include <random>
#include <vector>
#include <string>
#include "field.h"
#include "neoexpandedpolygon.h"
#include "expandedpolygon.h"

namespace bg = boost::geometry;
using point_t = bg::model::d2::point_xy<double>;
using ring_t = bg::model::ring<point_t>;
using polygon_t = bg::model::polygon<point_t,true,true,std::vector,std::vector,std::allocator,std::allocator>;

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
    QPointF getPosition(point_t point);
    void setField();
    std::vector<cv::Vec3b> colors;
    void setRandomColors(int threshold);
    int grid_size;
    int top_bottom_margin;
    int left_right_margin;
    int top_margin;
    procon::Field field;

protected:
    void beforePolygon();
    void paintEvent(QPaintEvent *event);
};

#endif // NEOANSWERBOARD_H
