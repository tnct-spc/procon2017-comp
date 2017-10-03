#ifndef NEOSINGLEPOLYGONRETOUCHER_H
#define NEOSINGLEPOLYGONRETOUCHER_H

#include <QWidget>
#include "expandedpolygon.h"

namespace Ui {
class NeoSinglePolygonRetoucher;
}

class NeoSinglePolygonRetoucher : public QWidget
{
    Q_OBJECT

public:
    static std::unique_ptr<NeoSinglePolygonRetoucher> createInstance(procon::ExpandedPolygon const& polygon_, cv::Mat const& image_, std::string wname_, int scale_);
    ~NeoSinglePolygonRetoucher();

protected:
    void paintEvent(QPaintEvent *);

private:
    explicit NeoSinglePolygonRetoucher(QWidget *parent = 0);
    Ui::NeoSinglePolygonRetoucher *ui;
    procon::ExpandedPolygon polygon;
    QImage image;
    std::string wname;
    int scale;
};

#endif // NEOSINGLEPOLYGONRETOUCHER_H
