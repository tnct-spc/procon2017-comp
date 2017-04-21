#include "neoexpandedpolygon.h"

#include <cmath>

procon::NeoExpandedPolygon::NeoExpandedPolygon()
{

}

// constructor
procon::NeoExpandedPolygon::NeoExpandedPolygon(int id_)
    : size(0),id(id_),inners_size(0)
{
    side_length.reserve(32);
    side_angle.reserve(32);
}

procon::NeoExpandedPolygon::NeoExpandedPolygon(std::vector<point> p)
{
    points = p;
}

//copy_constructor
procon::NeoExpandedPolygon::NeoExpandedPolygon(NeoExpandedPolygon const& p)
{
    this->id = p.id;
    this->size = p.size;
    this->points = p.points;
    this->inner_points = p.inner_points;
    this->inners_size = p.inners_size;
    // this->is_inverse = p.is_inverse;
    // this->jointed_pieces = p.jointed_pieces;
    // this->multi_ids = p.multi_ids;
    this->side_length = p.side_length;
    this->side_angle = p.side_angle;
    this->side_slope = p.side_slope;
    this->inners_side_length = p.inners_side_length;
    this->inners_side_angle = p.inners_side_angle;
    this->inners_side_slope = p.inners_side_slope;
    // this->polygon.outer().reserve(32);
    // this->centerx = p.centerx;
    // this->centery = p.centery;
    // this->difference_of_default_degree = p.difference_of_default_degree;

}

//--------------------private-----------------------
// 角の数
void procon::NeoExpandedPolygon::calcSize()
{
    size = points.size();
    inners_size = inner_points.size();
    calcSize_flag = true;
}

// getter
int procon::NeoExpandedPolygon::getSize() const
{
    return size;
}

int procon::NeoExpandedPolygon::getId() const
{
    return id;
}
