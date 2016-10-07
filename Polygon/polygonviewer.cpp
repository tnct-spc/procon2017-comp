#include "polygonviewer.h"

PolygonViewer::PolygonViewer(QWidget *parent) : QWidget(parent)
{

}

PolygonViewer& PolygonViewer::getInstance()
{
    static PolygonViewer instance;
    return instance;
}

void PolygonViewer::pushPolygon(procon::ExpandedPolygon const& polygon, std::string wname_, int scale)
{
    Displays.push_back(SinglePolygonDisplay::create(polygon, wname_, scale));
    (*(Displays.end()-1))->show();
}
