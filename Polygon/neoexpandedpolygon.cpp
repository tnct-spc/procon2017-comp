#include "neoexpandedpolygon.h"

#include <cmath>

procon::NeoExpandedPolygon::NeoExpandedPolygon()
{

}

void procon::NeoExpandedPolygon::setPolygon(polygon_i polygon)
{
    this->polygon = polygon;
}

polygon_i procon::NeoExpandedPolygon::getPolygon()
{
    return polygon;
}



