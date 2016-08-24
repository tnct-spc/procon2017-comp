#include "expandedpolygon.h"

//-------------------constructor--------------------
procon::ExpandedPolygon::ExpandedPolygon(int id_)
    : size(0),id(id_)
{
    //this->inners().push_back(polygon::ring_type());
    side_length.reserve(32);
    side_angle.reserve(32);
    polygon.outer().reserve(32);
}

//copy_constructor
procon::ExpandedPolygon::ExpandedPolygon(ExpandedPolygon const& p)
{
    this->id = p.id;
    this->polygon = p.polygon;
    this->size = p.size;
    std::copy(p.side_length.begin(),p_side_length.end(), std::back_inserter(this->side_length));
    std::copy(p.side_angle.begin(),p_side_angle.end(), std::back_inserter(this->side_angle));
    this->polygon.outer().reserve(32);

}

//--------------------private-----------------------
void procon::ExpandedPolygon::calcSize()
{
    size = static_cast<int>(polygon.outer().size() - 1);
    calcSize_flag = true;
}

void procon::ExpandedPolygon::calcSideLength()
{
    if(!calcSize_flag) calcSize();
    for(int i=0;i<size;i++){
       const double length = bg::distance(polygon.outer().at(i),polygon.outer().at(i+1));
       side_length.push_back(length);
    }
    calcSideLength_flag = true;
}

void procon::ExpandedPolygon::calcSideAngle()
{
    if(!calcSize_flag) calcSize();
    try {
        for (int i = -1;i < size - 1;i++){
            double x1,y1;
            if (i == -1){
                x1 = polygon.outer().at(size - 1).x(), y1 = polygon.outer().at(size - 1).y();
            } else {
                x1 = polygon.outer().at(i).x(), y1 = polygon.outer().at(i).y();
            }
            const double x2 = polygon.outer().at(i+1).x(), y2 = polygon.outer().at(i+1).y();
            const double x3 = polygon.outer().at(i+2).x(), y3 = polygon.outer().at(i+2).y();
            const double numer = (x2 - x1) * (x2 - x3) + (y2 - y1) * (y2 - y3);
            const double denom1 = std::sqrt(std::pow(x2 - x3,2) + std::pow(y2 - y3,2));
            const double denom2 = std::sqrt(std::pow(x2 - x1,2) + std::pow(y2 - y1,2));
            if (denom1 == 0 || denom2 == 0) {
                throw std::runtime_error("division by zero");
            }
            const double angle = std::acos(numer / (denom1 * denom2));
            side_angle.push_back(angle);
        }
    } catch (std::exception &e) {
           std::cerr << e.what() << std::endl;
    }
    calcSideAngle_flag = true;
}

//---------------------public------------------------
// getter
int procon::ExpandedPolygon::getSize()
{
    if(!calcSize_flag) calcSize();
    return size;
}

std::vector<double> const& procon::ExpandedPolygon::getSideLength()
{
    if(!calcSideLength_flag) calcSideLength();
    return side_length;
}

std::vector<double> const& procon::ExpandedPolygon::getSideAngle()
{
    if(!calcSideAngle_flag) calcSideAngle();
    return side_angle;
}

polygon_t const& procon::ExpandedPolygon::getPolygon() const
{
    return polygon;
}

int procon::ExpandedPolygon::getId() const
{
    return id;
}

//setter
void procon::ExpandedPolygon::setPolygon(polygon_t const& p)
{
    polygon = p;
}

// operator
procon::ExpandedPolygon procon::ExpandedPolygon::operator =
(procon::ExpandedPolygon const& p)
{
    this->id = p.id;
    this->polygon = p.polygon;
    std::copy(p.side_length.begin(),p_side_length.end(), std::back_inserter(this->side_length));
    std::copy(p.side_angle.begin(),p_side_angle.end(), std::back_inserter(this->side_angle));
    this->polygon.outer().reserve(32);
    this->size = p.size;
    return (*this);
}

// member

// 計算をまとめて行う
void procon::ExpandedPolygon::updatePolygon()
{
    calcSize();
    calcSideLength();
    calcSideAngle();
}
