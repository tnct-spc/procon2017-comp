#ifndef POLYGONVIEWER_H
#define POLYGONVIEWER_H

#include "expandedpolygon.h"
#include "field.h"
#include "singlepolygondisplay.h"
#include "neosinglepolygonretoucher.h"

class PolygonViewer : public QWidget
{
    Q_OBJECT
public:
    static PolygonViewer& getInstance();
    void pushPolygon(procon::ExpandedPolygon const& polygon, std::string wname_ = "", int scale = -1);
    void pushPolygon(procon::ExpandedPolygon const& polygon, cv::Mat const& image, std::string wname_ = "", int scale = -1);
private:
    explicit PolygonViewer(QWidget *parent = 0);
    std::vector<std::unique_ptr<SinglePolygonDisplay>> Displays;
    std::vector<std::unique_ptr<NeoSinglePolygonRetoucher>> Retouchers;

signals:

public slots:
};

#endif // POLYGONVIEWER_H
