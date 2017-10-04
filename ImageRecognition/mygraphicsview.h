#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsWidget>
#include "polygonviewer.h"
#include <QPaintEvent>

#include "../DisplayAnswer/neoanswerboard.h"

namespace Ui {
class MyGraphicsView;
}

class MyGraphicsView : public QWidget
{
    Q_OBJECT

public:
    explicit MyGraphicsView(QWidget *parent = 0);
    ~MyGraphicsView();

    void setPolygon(polygon_t polygon);
    polygon_t getPolygon();
    void setImage(cv::Mat image);

private:
    QPointF toPolygonPoint(double window_x,double window_y);
    QPoint toWindowPoint(QPointF point);
    void removePoint(int index, int x, int y);
    void insertPoint(int x,int y);
    void polygonUpdate();
    int isInTolerance(QPointF point);
    Ui::MyGraphicsView *ui;

    const double threshold = 0.1;//許容範囲
    const double magnification = 0.01;
    const int photo_margin = 10; //単位はpixel
    cv::Mat image;
    polygon_t polygon;
    std::vector<QPointF> points;
    double grid_size;
    int left_right_margin,top_buttom_margin;
    std::pair<double,double> minXY;
    bool selecting;
    int move_index;

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

};

#endif // MYGRAPHICSVIEW_H
