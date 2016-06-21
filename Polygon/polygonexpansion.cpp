#include "polygonexpansion.h"

PolygonExpansion::PolygonExpansion()
{
    //最大32角形
    sideLength.reserve(32);
    sideAngle.reserve(32);
}

polygon_t PolygonExpansion::getPolygon(){
    return polygon;
}

const std::vector<double> PolygonExpansion::getSideLength(){
    return sideLength;
}

const std::vector<double> PolygonExpansion::getSideAngle(){
    return sideAngle;
}

void PolygonExpansion::setPolygon(const polygon_t &pol){
    polygon = pol;
    calcN();
    calcSideLength();
    calcSideAngle();
}

void PolygonExpansion::calcN(){
    n = (int)(polygon.outer().end() - polygon.outer().begin() - 1);
}
void PolygonExpansion::calcSideLength(){
    for(int i=0;i<n;i++){
       sideLength.push_back(bg::distance(polygon.outer()[i],polygon.outer()[i+1]));
    }
}

void PolygonExpansion::calcSideAngle(){
    double x1 = bg::get<0>(polygon.outer()[n]),y1 = bg::get<1>(polygon.outer()[n]),x2 = bg::get<0>(polygon.outer()[0]),y2 = bg::get<1>(polygon.outer()[0]),x3 = bg::get<0>(polygon.outer()[1]),y3 = bg::get<1>(polygon.outer()[1]);
    if (y2 * y3 - y2 * y2 - y1 * y3 + y1 * y2 + x2 * x3 - x2 * x2 - x1 * x3 + x1 * x2 == 0) sideAngle.push_back(3.14159 / 2);
    else sideAngle.push_back(std::atan(std::sqrt(std::pow(-x3 * y2 + x3 * y1 - x2 * y1 + x2 * y3 - x1 * y3 + x1 * y2,2))) / std::sqrt(std::pow(y2 * y3 - y2 * y2 - y1 * y3 + y1 * y2 + x2 * x3 - x2 * x2 - x1 * x3 + x1 * x2,2)));
    for(int i=0;i<n-1;i++){
        x1 = bg::get<0>(polygon.outer()[i]),y1 = bg::get<1>(polygon.outer()[i]),x2 = bg::get<0>(polygon.outer()[i+1]),y2 = bg::get<1>(polygon.outer()[i+1]),x3 = bg::get<0>(polygon.outer()[i+2]),y3 = bg::get<1>(polygon.outer()[i+2]);
        if (y2 * y3 - y2 * y2 - y1 * y3 + y1 * y2 + x2 * x3 - x2 * x2 - x1 * x3 + x1 * x2 == 0) sideAngle.push_back(3.14159 / 2);
        else sideAngle.push_back(std::atan(std::sqrt(std::pow(-x3 * y2 + x3 * y1 - x2 * y1 + x2 * y3 - x1 * y3 + x1 * y2,2))) / std::sqrt(std::pow(y2 * y3 - y2 * y2 - y1 * y3 + y1 * y2 + x2 * x3 - x2 * x2 - x1 * x3 + x1 * x2,2)));
    }
}
