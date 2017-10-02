#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsWidget>
#include "polygonviewer.h"
namespace Ui {
class MyGraphicsView;
}

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit MyGraphicsView(QGraphicsView *parent = 0);
    ~MyGraphicsView();

    void setPolygon(polygon_t polygon);
    void setImage(cv::Mat mat);

private:
    QPointF toPolygonPoint(double window_x,double window_y);
    QPointF toWindowPoint(QPointF point);

    Ui::MyGraphicsView *ui;

    polygon_t polygon;
    std::vector<QPointF> points;
    int grid_size,left_right_margin,top_buttom_margin;
    std::pair<double,double> minXY;
    bool selecting;
    int move_index;

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

};

#endif // MYGRAPHICSVIEW_H
