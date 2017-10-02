#ifndef IMAGERECONGNITIONWITHHUMANPOWER_H
#define IMAGERECONGNITIONWITHHUMANPOWER_H

#include <QWidget>
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
    void setPolygon(polygon_t polygon);

private:
    Ui::imagerecongnitionwithhumanpower *ui;
    MyGraphicsView *my_graphics_view;
    polygon_t polygon;

private slots:

protected:

signals:

};

#endif // IMAGERECONGNITIONWITHHUMANPOWER_H
