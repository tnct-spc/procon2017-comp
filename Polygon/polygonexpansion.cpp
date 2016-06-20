#include "polygonexpansion.h"

PolygonExpansion::PolygonExpansion()
{
    sideLength.reserve(30);
    sideAngle.reserve(30);
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
    //calcsideAngle();
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
    /*
    int size = (int)(polygon.outer().end() - polygon.outer().begin() - 1);
    sideAngle.push_back(std::acos(sideLength[0] / sideLength[size-1]));
    for(int i=1;i<size;i++){
        sideAngle.push_back();
    }
    */
}
