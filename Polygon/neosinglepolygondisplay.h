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
<<<<<<< HEAD
    static std::unique_ptr<NeoSinglePolygonDisplay> createInstance(QWidget *widget,polygon_i const& print_polygon,std::string window_name);
    explicit NeoSinglePolygonDisplay(QWidget *parent = 0);
    ~NeoSinglePolygonDisplay();
    void setWindowName(std::string window_name);
=======
    static std::unique_ptr<NeoSinglePolygonDisplay> createInstance(polygon_i const& print_polygon,std::string window_name);
    explicit NeoSinglePolygonDisplay(QWidget *parent = 0);
    ~NeoSinglePolygonDisplay();
>>>>>>> develop
    void setPolygon(polygon_i polygon);

private:
    Ui::NeoSinglePolygonDisplay *ui;
    polygon_i polygon;

protected:
    void paintEvent(QPaintEvent *);
};

#endif // NEOSINGLEPOLYGONDISPLAY_H
