#ifndef IMAGERECONGNITIONWITHHUMANPOWER_H
#define IMAGERECONGNITIONWITHHUMANPOWER_H

#include <QWidget>
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

    void setPolygon(polygon_t polygont);

private:
    Ui::imagerecongnitionwithhumanpower *ui;

    procon::ExpandedPolygon polygonAdapter(polygon_t polygont);
    polygon_t polygonAdapter(procon::ExpandedPolygon polygont);
    QPushButton *edited_button;
    polygon_t polygont;
    procon::ExpandedPolygon polygon;
    std::vector<int> grid_x;
    std::vector<int> grid_y;

private slots:
    void clickedEditedButton();
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // IMAGERECONGNITIONWITHHUMANPOWER_H
