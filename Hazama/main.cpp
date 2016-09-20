#include "hazama.h"
#include "polygonviewer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    PolygonViewer::getInstance();

    Hazama w;
    w.init();
    w.show();

    return a.exec();
}
