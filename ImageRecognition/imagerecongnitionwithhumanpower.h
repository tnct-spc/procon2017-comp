#ifndef IMAGERECONGNITIONWITHHUMANPOWER_H
#define IMAGERECONGNITIONWITHHUMANPOWER_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QLayout>
#include <QSpacerItem>
#include <QString>
#include "precompile.h"
#include "polygonviewer.h"
#include "singlepolygondisplay.h"

namespace Ui {
class imagerecongnitionwithhumanpower;
}

class imagerecongnitionwithhumanpower : public QWidget
{
    Q_OBJECT

public:
    explicit imagerecongnitionwithhumanpower(QWidget *parent = 0);
    ~imagerecongnitionwithhumanpower();

    void setPolygon(polygon_t polygon);
    void setImage(QString path);
    void setImage(cv::Mat mat);

private:
    QPointF toPolygonPoint(double window_x,double window_y);
    QPointF toWindowPoint(QPointF point);
    Ui::imagerecongnitionwithhumanpower *ui;
    polygon_t polygon;
    std::vector<QPointF> points;
    int grid_size,left_right_margin,top_buttom_margin;
    std::pair<double,double> minXY;
    bool selecting;
    int move_index;

private slots:
    void clickedEditedButton();
    void clickedEnlargement();
    void clickedReduction();
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void returnPolygon(polygon_t polygon);
};

#endif // IMAGERECONGNITIONWITHHUMANPOWER_H
