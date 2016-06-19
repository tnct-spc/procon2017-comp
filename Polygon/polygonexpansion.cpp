#include "polygonexpansion.h"

PolygonExpansion::PolygonExpansion()
{
    sideLength.reserve(30);
    sideAngle.reserve(30);
}

polygon_t &PolygonExpansion::getPolygon(){
    return polygon;
}

void PolygonExpansion::setPolygon(const polygon_t &pol){
    polygon = pol;
}

void PolygonExpansion::calcSideLength(){
    //for(auto itPol = polygon.outer().begin(),auto itLen = sideLength.begin();itPol != polygon.outer().end();itPol++,itLen++){
       // itLen = bg::distance(itPol,itPol+1);
    //}
}
