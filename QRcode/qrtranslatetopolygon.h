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

    std::string inputqr;
    std::vector<std::string> qrinputvector;
    std::vector<int> colonvector;
    int frame_size;

    void inpToVector();
    void translateToDoubleArray();
    /*
    void findColon();
    void splitQrInput();
    void splitBasisOfSpace(std::string &str,std::vector<int> &qrvec);
    void translateToPolygon(std::vector<int> &intvec,polygon_i &polygon);
    std::vector<std::string> splitedqrinput;
    std::vector<std::string> framestring;
    std::vector<int> colon;
    std::vector<std::vector<int>> qrvector;
    std::vector<std::vector<int>> framevector;
    std::string qrinput;
    std::vector<polygon_i> polygon;
    std::vector<polygon_i> framepolygon;
    int shapecount;
    bool useframedata = false;
    int framedatacount = 0;
    */
public:
    QrTranslateToPolygon(std::string qrinp);
    /*
    std::vector<polygon_i> getPieceData();
    std::vector<polygon_i> getFrameData();
    static void translateToCSV(std::vector<polygon_i> const& piece, const std::vector<polygon_i> &frame);

    int get();
    */
};

#endif // QRTRANSLATETOPOLYGON_H
