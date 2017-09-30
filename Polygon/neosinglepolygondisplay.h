#ifndef NEOSINGLEPOLYGONDISPLAY_H
#define NEOSINGLEPOLYGONDISPLAY_H

#include "neoexpandedpolygon.h"

#include <QWidget>
#include <memory>
#include <string>
#include <QPoint>

namespace Ui {
class NeoSinglePolygonDisplay;
}

class NeoSinglePolygonDisplay : public QWidget
{
    Q_OBJECT

public:
    static std::unique_ptr<NeoSinglePolygonDisplay> createInstance(polygon_i const& print_polygon,std::string window_name, bool enlarged_polygon);
    explicit NeoSinglePolygonDisplay(QWidget *parent = 0);
    ~NeoSinglePolygonDisplay();
    void setPolygon(polygon_i polygon);
    void setIsEnlargedPolygon(bool is_enlarged_polygon);

private:
    Ui::NeoSinglePolygonDisplay *ui;
    polygon_i polygon;
    bool enlarged_polygon;
    std::vector<double> angles;

protected:
    void paintEvent(QPaintEvent *);
};

#endif // NEOSINGLEPOLYGONDISPLAY_H
