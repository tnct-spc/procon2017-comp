#include "probmaker.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*test code*/
    /*
    procon::polygon::ExpandedPolygon p;
    p.outer().push_back(point_t(0,0));
    p.outer().push_back(point_t(0,1));
    p.outer().push_back(point_t(1,0));
    p.outer().push_back(point_t(0,0));
    p.updatePolygon();
    auto sl = p.getSideAngle();
    auto sa = p.getSideLength();
    auto ps = p.getSize();
    for (auto l : sl) std::cout << l << std::endl;
    for (auto a : sa) std::cout << a << std::endl;
    std::cout << ps << std::endl;
    std::cout << "Hello,world!" << std::endl;
    std::cout << bg::area(p) << std::endl;
    std::cout << bg::dsv(p) << std::endl;
    std::cout << p.outer().at(2).x() << std::endl;
    */
    ProbMaker w;
    w.show();
    w.run();
    return a.exec();
}
