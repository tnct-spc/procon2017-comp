#include "neopolygonviewer.h"

NeoPolygonViewer::NeoPolygonViewer(QWidget *parent) : QWidget(parent)
{

}

NeoPolygonViewer& NeoPolygonViewer::getInstance()
{
    static NeoPolygonViewer instance;
    return instance;
}

void NeoPolygonViewer::displayPolygon(const polygon_i &polygon, std::string wname)
{
    viewers.push_back(NeoSinglePolygonDisplay::createInstance(polygon,wname));
    viewers.at(viewers.size() - 1)->show();
}
