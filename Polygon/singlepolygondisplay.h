#ifndef SINGLEPOLYGONDISPLAY_H
#define SINGLEPOLYGONDISPLAY_H

#include "expandedpolygon.h"

namespace Ui {
class SinglePolygonDisplay;
}

class SinglePolygonDisplay : public QWidget
{
    Q_OBJECT

public:
    static std::unique_ptr<SinglePolygonDisplay> create(procon::ExpandedPolygon const& polygon, std::string wname_, int scale);
    ~SinglePolygonDisplay();

private:
    explicit SinglePolygonDisplay(QWidget *parent);
    Ui::SinglePolygonDisplay *ui;
    procon::ExpandedPolygon polygon;
    int scale;

protected:
    void paintEvent(QPaintEvent *);
};

#endif // SINGLEPOLYGONDISPLAY_H
