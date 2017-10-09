#ifndef IMAGERECONGNITIONWITHHUMANPOWER_H
#define IMAGERECONGNITIONWITHHUMANPOWER_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "mygraphicsview.h"
#include "precompile.h"
#include "polygonviewer.h"

namespace Ui {
class imagerecongnitionwithhumanpower;
}

class imagerecongnitionwithhumanpower : public QWidget
{
    Q_OBJECT

public:
    explicit imagerecongnitionwithhumanpower(QWidget *parent = 0);
    ~imagerecongnitionwithhumanpower();
    void setPolygon(polygon_t const& polygon);
    void setImage(cv::Mat const& image);

signals:
    void updatePiece(polygon_t const& piece, int piece_number);

private:
    Ui::imagerecongnitionwithhumanpower *ui;
    QGraphicsView *view;
    QGraphicsScene *scene;
    MyGraphicsView *my_graphics_view;

private slots:
    void clickedEditedButton();
    void clickedEnlargementButton();
    void clickedReductionButton();
protected:

signals:
    void returnPolygon(polygon_t polygon);
};

#endif // IMAGERECONGNITIONWITHHUMANPOWER_H
