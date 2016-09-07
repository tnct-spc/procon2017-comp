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
    std::copy(p.side_length.begin(),p.side_length.end(), std::back_inserter(this->side_length));
    std::copy(p.side_angle.begin(),p.side_angle.end(), std::back_inserter(this->side_angle));
    std::copy(p.side_slope.begin(),p.side_slope.end(), std::back_inserter(this->side_slope));
    this->polygon.outer().reserve(32);
    this->centerx = p.centerx;
    this->centery = p.centery;
    this->difference_of_default_degree = p.difference_of_default_degree;

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
    side_length.clear();
    for(int i=0;i < size;i++){
       const double length = bg::distance(polygon.outer().at(i),polygon.outer().at(i+1));
       side_length.push_back(length);
    }
}

void procon::ExpandedPolygon::calcSideAngle()
{
    auto isMinorAngle = [&](point_t p1,point_t p2,point_t p3)->bool {
        point_t p4((p1.x() + p3.x()) / 2 , (p1.y() + p3.y()) / 2);
        const double alpha = 4000;
        const double end_x = (p4.x() - p2.x()) > 0 ? alpha : -alpha;
        const double end_y = (p4.y() - p2.y()) / (p4.x() - p2.x()) * (end_x - p2.x()) + p2.y();
        point_t end_point(end_x,end_y);
        bg::model::segment<point_t> judge_segment(p2,end_point);
        int intersect_num = 0;

        for (int i = 0;i < size;i++) {
            point_t pa = polygon.outer().at(i);
            point_t pb = polygon.outer().at(i+1);
            bg::model::segment<point_t> side_segment(pa,pb);
            intersect_num += static_cast<int>(bg::intersects(side_segment,judge_segment));
        }

        return (intersect_num % 2 == 0) ? true : false;
    };

    if(!calcSize_flag) calcSize();
    side_angle.clear();
    try {
        for (int i = -1;i < size - 1;i++){
            double x1,y1;
            point_t p1;
            if (i == -1){
                x1 = polygon.outer().at(size - 1).x(), y1 = polygon.outer().at(size - 1).y();
                p1 = polygon.outer().at(size - 1);
            } else {
                x1 = polygon.outer().at(i).x(), y1 = polygon.outer().at(i).y();
                p1 = polygon.outer().at(i);
            }
            const double x2 = polygon.outer().at(i+1).x(), y2 = polygon.outer().at(i+1).y();
            const double x3 = polygon.outer().at(i+2).x(), y3 = polygon.outer().at(i+2).y();
            const point_t p2 = polygon.outer().at(i+1);
            const point_t p3 = polygon.outer().at(i+2);
            const double numer = (x2 - x1) * (x2 - x3) + (y2 - y1) * (y2 - y3);
            const double denom1 = std::sqrt(std::pow(x2 - x3,2) + std::pow(y2 - y3,2));
            const double denom2 = std::sqrt(std::pow(x2 - x1,2) + std::pow(y2 - y1,2));
            if (denom1 == 0 || denom2 == 0) {
                throw std::runtime_error("division by zero");
            }
            double angle = std::acos(numer / (denom1 * denom2));

            if (isMinorAngle(p1,p2,p3)) {
                angle = (3.141592 * 2) - angle;
            }
            side_angle.push_back(angle);
        }
    } catch (std::exception &e) {
           std::cerr << e.what() << std::endl;
    }
}

void procon::ExpandedPolygon::calcSideSlope() {
    calcSize();
    side_slope.clear();
    for (int i = 0;i < size;i++) {
        const double x = polygon.outer().at(i).x() - polygon.outer().at(i + 1).x();
        const double y = std::abs(polygon.outer().at(i).y() - polygon.outer().at(i + 1).y());
        side_slope.push_back(std::atan2(y,x));
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

std::vector<double> const& procon::ExpandedPolygon::getSideSlope() const
{
    return side_slope;
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
    this->updatePolygon(true);
}

// operator
procon::ExpandedPolygon procon::ExpandedPolygon::operator =
(procon::ExpandedPolygon const& p)
{
    this->id = p.id;
    this->polygon = p.polygon;
    std::copy(p.side_length.begin(),p.side_length.end(), std::back_inserter(this->side_length));
    std::copy(p.side_angle.begin(),p.side_angle.end(), std::back_inserter(this->side_angle));
    std::copy(p.side_slope.begin(),p.side_slope.end(), std::back_inserter(this->side_slope));
    this->polygon.outer().reserve(32);
    this->size = p.size;
    this->centerx = p.centerx;
    this->centery = p.centery;
    this->difference_of_default_degree = p.difference_of_default_degree;

    return (*this);
}

// member

// 計算をまとめて行う
void procon::ExpandedPolygon::updatePolygon(bool calc)
{
    if(calc) {
        calcSize();
        calcSideLength();
        calcSideAngle();
        calcSideSlope();
    } else {
        std::cout << "updatePolygonは必要なくなったよ" << std::endl;
    }
}

void procon::ExpandedPolygon::inversePolygon()
{
    polygon_t translate_polygon;

    boost::geometry::strategy::transform::translate_transformer<double,2,2> transformgo(-centerx,0);
    boost::geometry::transform(polygon,translate_polygon,transformgo);

    //
    polygon_t inversedPolygon;
    const int polygon_size = polygon.outer().size();
    for(int i=0; i < polygon_size; i++){
        const double x = translate_polygon.outer().at(i).x();
        const double y = translate_polygon.outer().at(i).y();
        inversedPolygon.outer().push_back(boost::geometry::model::d2::point_xy<double>(-x,y));
    }

    polygon_t returnPolygon;

    boost::geometry::strategy::transform::translate_transformer<double,2,2> transformback(centerx,0);
    boost::geometry::transform(inversedPolygon,returnPolygon,transformback);
    std::reverse(returnPolygon.outer().begin(),returnPolygon.outer().end());
    polygon = returnPolygon;
}

void procon::ExpandedPolygon::rotatePolygon(double degree)
{
    polygon_t goPolygon;
    boost::geometry::strategy::transform::translate_transformer<double,2,2> goTranslate(-centerx,-centery);
    boost::geometry::transform(polygon,goPolygon,goTranslate);

    polygon_t rotated_Polygon;
    boost::geometry::strategy::transform::rotate_transformer<boost::geometry::degree,double,2,2> rotate(degree);

    boost::geometry::transform(goPolygon,rotated_Polygon,rotate);

    polygon_t backPolygon;
    boost::geometry::strategy::transform::translate_transformer<double,2,2> backTranslate(centerx,centery);
    boost::geometry::transform(rotated_Polygon,backPolygon,backTranslate);

    difference_of_default_degree = difference_of_default_degree + degree;

    polygon = backPolygon;

    constexpr double pi = 3.1415926535;
    constexpr double to_rad = pi / 180;
    for (auto & slope : side_slope) {
        slope += degree * to_rad;
        if (slope > pi) slope -= pi;
    }
}

void procon::ExpandedPolygon::translatePolygon(double x, double y)
{
    polygon_t translatedPolygon;
    boost::geometry::strategy::transform::translate_transformer<double,2,2> backTranslate(x,y);
    boost::geometry::transform(polygon,translatedPolygon,backTranslate);
    

    centerx = centerx + x;
    centery = centery + y;

    polygon = translatedPolygon;
}

void procon::ExpandedPolygon::setPolygonAngle(double degree)
{
    polygon_t goPolygon;
    boost::geometry::strategy::transform::translate_transformer<double,2,2> goTranslate(-centerx,-centery);
    boost::geometry::transform(polygon,goPolygon,goTranslate);

    //回転量
    double ddegree = degree - difference_of_default_degree;

    while(ddegree < 0){
        ddegree = ddegree + 360;
    }

    polygon_t rotatedPolygon;
    boost::geometry::strategy::transform::rotate_transformer<boost::geometry::degree,double,2,2> rotate(ddegree);
    boost::geometry::transform(goPolygon,rotatedPolygon,rotate);

    polygon_t backPolygon;
    boost::geometry::strategy::transform::translate_transformer<double,2,2> backTranslate(centerx,centery);
    boost::geometry::transform(rotatedPolygon,backPolygon,backTranslate);

    difference_of_default_degree = degree;

    polygon = backPolygon;

    constexpr double pi = 3.1415926535;
    constexpr double to_rad = pi / 180;
    for (auto & slope : side_slope){
        slope += degree * to_rad;
        if (slope > pi) slope -= pi;
    }
}

void procon::ExpandedPolygon::setPolygonPosition(double x, double y)
{
    double dx = x - centerx;
    double dy = y - centery;
    
    polygon_t translatedPolygon;

    boost::geometry::strategy::transform::translate_transformer<double,2,2> translate(dx,dy);
    boost::geometry::transform(polygon,translatedPolygon,translate);
    
    centerx = x;
    centery = y;
    
    polygon = translatedPolygon;
}
