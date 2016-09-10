#include "polygonviewer.h"

PolygonViewer::PolygonViewer(QWidget *parent) : QWidget(parent)
{

}

PolygonViewer& PolygonViewer::getInstance()
{
    static PolygonViewer instance;
    return instance;
}

void PolygonViewer::pushPolygon(procon::ExpandedPolygon& polygon, int scale, std::string wname_)
{
    Displays.push_back(SinglePolygonDisplay::create(polygon, scale, wname_));
    (*(Displays.end()-1))->show();
}
