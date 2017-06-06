#include "neoexpandedpolygon.h"

#include <cmath>

NeoExpandedPolygon::NeoExpandedPolygon()
{

}

void NeoExpandedPolygon::setPolygon(polygon_i polygon)
{
    this->polygon = polygon;
}

polygon_i NeoExpandedPolygon::getPolygon()
{
    return polygon;
}



