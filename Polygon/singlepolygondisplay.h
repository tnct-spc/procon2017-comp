#ifndef SINGLEPOLYGONDISPLAY_H
#define SINGLEPOLYGONDISPLAY_H

#include <QWidget>
#include "expandedpolygon.h"

namespace Ui {
class SinglePolygonDisplay;
}

class SinglePolygonDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit SinglePolygonDisplay(QWidget *parent = 0);
    ~SinglePolygonDisplay();
    void setPolygon(procon::ExpandedPolygon& polygon, int scale = 30);

private:
    Ui::SinglePolygonDisplay *ui;
    procon::ExpandedPolygon polygon;
    int scale;

protected:
    void paintEvent(QPaintEvent *);
};

#endif // SINGLEPOLYGONDISPLAY_H
