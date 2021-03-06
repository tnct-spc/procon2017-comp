#include "probmaker.h"
#include <QApplication>
#include "neopolygonviewer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ProbMaker w;
    w.show();

    polygon_i polygon;
    polygon.outer().push_back(point_i(0,0));
    polygon.outer().push_back(point_i(3,0));
    polygon.outer().push_back(point_i(3,3));
    polygon.outer().push_back(point_i(0,0));
    procon::NeoExpandedPolygon polygonn;
    polygonn.resetPolygonForce(polygon);

    NeoPolygonViewer::getInstance();

//    auto aa = NeoSinglePolygonDisplay::createInstance(polygon,"hogehoge",false);
//    aa->show();

    return a.exec();
}
