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

    void setPolygon(polygon_t polygon);

private:
    procon::ExpandedPolygon polygonAdapter(polygon_t polygon);
    polygon_t polygonAdapter(procon::ExpandedPolygon polygon);
    Ui::imagerecongnitionwithhumanpower *ui;
    QPushButton *edited_button;
    polygon_t polygon;

private slots:
    void clickedEditedButton();
};

#endif // IMAGERECONGNITIONWITHHUMANPOWER_H
