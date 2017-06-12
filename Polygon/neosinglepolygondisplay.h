#ifndef NEOSINGLEPOLYGONDISPLAY_H
#define NEOSINGLEPOLYGONDISPLAY_H

#include "neoexpandedpolygon.h"

#include <QWidget>
#include <memory>
#include <string>

namespace Ui {
class NeoSinglePolygonDisplay;
}

class NeoSinglePolygonDisplay : public QWidget
{
    Q_OBJECT

public:
    static std::unique_ptr<NeoSinglePolygonDisplay> createInstance(procon::NeoExpandedPolygon const& print_polygon,std::string window_name);
    explicit NeoSinglePolygonDisplay(QWidget *parent = 0);
    ~NeoSinglePolygonDisplay();

private:
    Ui::NeoSinglePolygonDisplay *ui;
    procon::NeoExpandedPolygon polygon;

protected:
    void paintEvent(QPaintEvent *);
};

#endif // NEOSINGLEPOLYGONDISPLAY_H
