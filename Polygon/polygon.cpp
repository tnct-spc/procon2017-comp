#include "polygon.h"

Polygon::Polygon()
{
}

void Polygon::setPolygon(const polygon_t &tmp){
    mizuki_chan.push_back(tmp);
}

void Polygon::setPointXy(const std::vector<point_xy_t> &tmp){
    for_each(tmp.begin(),tmp.end(),[&](const point_xy_t &p)->void{bg::exterior_ring(mizuki_chan.back()) = boost::assign::list_of<point_xy_t>(p);});
}
