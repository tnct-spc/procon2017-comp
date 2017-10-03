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
    void setPolygon(polygon_t const& polygon);
    void setImage(cv::Mat const& image);

private:
    Ui::imagerecongnitionwithhumanpower *ui;
    MyGraphicsView *my_graphics_view;

private slots:
    void clickedEnlargement();
    void clickedReduction();
protected:

signals:

};

#endif // IMAGERECONGNITIONWITHHUMANPOWER_H
