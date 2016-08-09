#include "expandedpolygon.h"

//-------------------constructor--------------------
procon::ExpandedPolygon::ExpandedPolygon()
    : size(0)
{
    //this->inners().push_back(polygon::ring_type());
    side_length.reserve(32);
    side_angle.reserve(32);
    polygon.outer().reserve(32);
}

//copy_constructor
procon::ExpandedPolygon::ExpandedPolygon(ExpandedPolygon const& p)
{
    this->polygon = p.polygon;
    this->size = p.size;
    this->side_length = p.side_length;
    this->side_angle = p.side_angle;
    this->side_length.reserve(32);
    this->side_angle.reserve(32);
    this->polygon.outer().reserve(32);

}

//--------------------private-----------------------
void procon::ExpandedPolygon::calcSize()
{
    size = static_cast<int>(polygon.outer().size() - 1);
}

void procon::ExpandedPolygon::calcSideLength()
{
    for(int i=0;i<size;i++){
       const double length = bg::distance(polygon.outer().at(i),polygon.outer().at(i+1));
       side_length.push_back(length);
    }
}

void procon::ExpandedPolygon::calcSideAngle()
{
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
}

//---------------------public------------------------
// getter
int procon::ExpandedPolygon::getSize() const
{
    return size;
}

std::vector<double> const& procon::ExpandedPolygon::getSideLength() const
{
    return side_length;
}

std::vector<double> const& procon::ExpandedPolygon::getSideAngle() const
{
    return side_angle;
}

polygon_t const& procon::ExpandedPolygon::getPolygon() const
{
    return polygon;
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
    this->polygon = p.polygon;
    this->side_length = p.side_length;
    this->side_angle = p.side_angle;
    this->side_length.reserve(32);
    this->side_angle.reserve(32);
    this->polygon.outer().reserve(32);
    return (*this);
}

// member

// 計算をまとめて行う
// ここ以外からcalc関数を呼ぶのは禁止
void procon::ExpandedPolygon::updatePolygon()
{
    calcSize();
    calcSideLength();
    calcSideAngle();
}
