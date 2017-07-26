#ifndef QRTRANSLATETOPOLYGON_H
#define QRTRANSLATETOPOLYGON_H

#include <iostream>
#include <string>
#include <vector>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/strategies/transform.hpp>
#include <boost/geometry/strategies/transform/matrix_transformers.hpp>
#include <boost/geometry/algorithms/disjoint.hpp>
#include "neoexpandedpolygon.h"

class QrTranslateToPolygon
{
private:
    void findColon();
    void splitQrInput();
    void splitBasisOfSpace(std::string &str,std::vector<int> &qrvec);
    void translateToPolygon(std::vector<int> &intvec,polygon_i &polygon);
    std::vector<std::string> splitedqrinput;
    std::string framestring;
    std::vector<int> colon;
    std::vector<std::vector<int>> qrvector;
    std::vector<int> framevector;
    std::string qrinput;
    std::vector<polygon_i> polygon;
    polygon_i framepolygon;
    int shapecount;
    bool useframedata = false;
public:
    std::vector<polygon_i> getPieceData();
    polygon_i getFrameData();
    QrTranslateToPolygon(std::string qrinp);

    int get();
};

#endif // QRTRANSLATETOPOLYGON_H
