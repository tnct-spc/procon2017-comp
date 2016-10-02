#include "expandedpolygon.h"

#include "utilities.h"
#include "utility"

//-------------------constructor--------------------
procon::ExpandedPolygon::ExpandedPolygon(int id_)
    : size(0),id(id_),inners_size(0)
{
    //this->inners().push_back(polygon::ring_type());
    side_length.reserve(32);
    side_angle.reserve(32);
    polygon.outer().reserve(32);
}
procon::ExpandedPolygon::ExpandedPolygon(std::vector<int> multi_ids_)
    : size(0),id(-1)
{
    //this->inners().push_back(polygon::ring_type());
    side_length.reserve(32);
    side_angle.reserve(32);
    polygon.outer().reserve(32);

    std::copy(multi_ids_.begin(),multi_ids_.end(), std::back_inserter(multi_ids));
    while(0){}
}

//copy_constructor
procon::ExpandedPolygon::ExpandedPolygon(ExpandedPolygon const& p)
{
    this->id = p.id;
    this->polygon = p.polygon;
    this->size = p.size;
    this->inners_size = p.inners_size;
    this->is_inverse = p.is_inverse;
    this->jointed_pieces = p.jointed_pieces;
    this->multi_ids = p.multi_ids;
    this->side_length = p.side_length;
    this->side_angle = p.side_angle;
    this->side_slope = p.side_slope;
    this->inners_side_length = p.inners_side_length;
    this->inners_side_angle = p.inners_side_angle;
    this->inners_side_slope = p.inners_side_slope;
    this->polygon.outer().reserve(32);
    this->centerx = p.centerx;
    this->centery = p.centery;
    this->difference_of_default_degree = p.difference_of_default_degree;

}

//--------------------private-----------------------
void procon::ExpandedPolygon::calcSize()
{
    size = static_cast<int>(polygon.outer().size() - 1);
    inners_size = static_cast<int>(polygon.inners().size());
    calcSize_flag = true;
}

void procon::ExpandedPolygon::calcSideLength()
{
    if(!calcSize_flag) calcSize();
    side_length.clear();
    inners_side_length.clear();

    for(int i=0;i < size;i++){
        const double length = bg::distance(polygon.outer().at(i),polygon.outer().at(i+1));
        side_length.push_back(length);
    }

    for(auto inner : polygon.inners()){
        std::vector<double> inner_side_length;
        for(int i=0;i < static_cast<int>(inner.size() - 1);i++){
            const double length = bg::distance(inner.at(i),inner.at(i+1));
            inner_side_length.push_back(length);
        }
        inners_side_length.push_back(inner_side_length);
    }
}

void procon::ExpandedPolygon::calcSideAngle()
{
    auto calcAngle = [&](point_t p1,point_t p2,point_t p3,int pos,std::vector<point_t> inner = {})->double{

        auto isMajorAngle = [&](point_t p1,point_t p2,point_t p3)->bool {
            point_t p4((p1.x() + p3.x()) / 2 , (p1.y() + p3.y()) / 2);
            const double alpha = 4000.000000283865;
            const double beta  = 0.000001446343;
            const double ganma = 0.000002532443;
            const double end_x = (p4.x() - p2.x() + ganma) > 0 ? alpha : -alpha;
            const double end_y_length = abs((p4.y() - p2.y()) / (p4.x() - p2.x() + ganma) * (end_x - p2.x()) + p2.y() + beta);
            const double end_y = p4.y() - p2.y() > 0 ? end_y_length : -end_y_length;
            point_t end_point(end_x,end_y);
            //bg::model::segment<point_t> judge_segment(p2,end_point);
            int intersect_num = 0,points_size = size;
            if (!inner.empty()){
                points_size = static_cast<int>(inner.size() - 1);
            }
            for (int i = 0;i < points_size;i++) {
                // skip tonari
                if(i==pos || i+1 == pos || (pos==0 && i==points_size-1)) continue;
                point_t pa,pb;
                if (!inner.empty()) {
                    pa = inner.at(i);
                    pb = inner.at(i+1);
                } else {
                    pa = polygon.outer().at(i);
                    pb = polygon.outer().at(i+1);
                }
                //bg::model::segment<point_t> side_segment(pa,pb);
                intersect_num += static_cast<int>(Utilities::cross_check(p2,end_point,pa,pb));
                //intersect_num += static_cast<int>(bg::intersects(side_segment,judge_segment));
            }

            return (intersect_num % 2 == 0) ? true : false;
        };

        double angle = 0;
        try {
            const double numer = (p2.x() - p1.x()) * (p2.x() - p3.x()) + (p2.y() - p1.y()) * (p2.y() - p3.y());
            const double denom1 = std::sqrt(std::pow(p2.x() - p3.x(),2) + std::pow(p2.y() - p3.y(),2));
            const double denom2 = std::sqrt(std::pow(p2.x() - p1.x(),2) + std::pow(p2.y() - p1.y(),2));
            if (denom1 == 0 || denom2 == 0) {
                throw std::runtime_error("division by zero");
            }

            angle = std::acos(numer / (denom1 * denom2));

            if (isMajorAngle(p1,p2,p3)) {
                angle = (3.141592 * 2) - angle;
            }

        } catch (std::exception &e) {
           std::cerr << e.what() << std::endl;
        }
        return angle;
    };

    if(!calcSize_flag) calcSize();
    side_angle.clear();
    inners_side_angle.clear();

    for (int i = -1;i < size - 1;i++){
        point_t p1;
        if (i == -1){
            p1 = polygon.outer().at(size - 1);
        } else {
            p1 = polygon.outer().at(i);
        }
        const point_t p2 = polygon.outer().at(i+1);
        const point_t p3 = polygon.outer().at(i+2);
        side_angle.push_back(calcAngle(p1,p2,p3,i+1));
    }

    for (auto inner : polygon.inners()) {
        std::vector<double> inner_side_angle;
        const int inner_size = static_cast<int>(inner.size() - 1);
        for (int i = -1;i < inner_size - 1;i++){
            point_t p1;
            if (i == -1){
                p1 = inner.at(inner_size - 1);
            } else {
                p1 = inner.at(i);
            }
            const point_t p2 = inner.at(i+1);
            const point_t p3 = inner.at(i+2);
            inner_side_angle.push_back(calcAngle(p1,p2,p3,i+1,inner));
        }
        inners_side_angle.push_back(inner_side_angle);
    }

}

void procon::ExpandedPolygon::calcSideSlope() {
    calcSize();
    side_slope.clear();
    inners_side_slope.clear();

    for (int i = 0;i < size;i++) {
        const double x = polygon.outer().at(i).x() - polygon.outer().at(i + 1).x();
        const double y = std::abs(polygon.outer().at(i).y() - polygon.outer().at(i + 1).y());
        side_slope.push_back(std::atan2(y,x));
    }

    for (auto inner : polygon.inners()){
        std::vector<double> inner_side_slope;
        for (int i = 0;i < static_cast<int>(inner.size() - 1);i++) {
            const double x = inner.at(i).x() - inner.at(i + 1).x();
            const double y = std::abs(inner.at(i).y() - inner.at(i + 1).y());
            inner_side_slope.push_back(std::atan2(y,x));
        }
        inners_side_slope.push_back(inner_side_slope);
    }
}

//---------------------public------------------------
// getter
int procon::ExpandedPolygon::getSize() const
{
    return size;
}

int procon::ExpandedPolygon::getInnerSize() const
{
    return inners_size;
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

std::vector<int> procon::ExpandedPolygon::getMultiIds() const
{
    return multi_ids;
}

std::string procon::ExpandedPolygon::makeMultiIdString() const
{
    std::string id_string = "";
    for(int i=0; i < static_cast<int>(multi_ids.size()) && (i == 0  || (id_string += "+") != ""); ++i){
        id_string += std::to_string(multi_ids.at(i));
    }
    return id_string;
}

std::vector<std::vector<double>> const& procon::ExpandedPolygon::getInnersSideLength() const
{
    return inners_side_length;
}

std::vector<std::vector<double>> const& procon::ExpandedPolygon::getInnersSideAngle() const
{
    return inners_side_angle;
}

std::vector<std::vector<double>> const& procon::ExpandedPolygon::getInnersSideSlope() const
{
    return inners_side_slope;
}

std::vector<procon::ExpandedPolygon> const& procon::ExpandedPolygon::getJointedPieces() const
{
    return jointed_pieces;
}

//setter
void procon::ExpandedPolygon::setMultiIds(std::vector<int> multi_ids_)
{
    multi_ids = multi_ids_;
}

void procon::ExpandedPolygon::resetPolygonForce(polygon_t const& p)
{
    polygon = p;
    this->updatePolygon(true);

    // Reset join data
    jointed_pieces.clear();
}

void procon::ExpandedPolygon::pushNewJointedPolygon(const polygon_t &new_frame, const procon::ExpandedPolygon &jointed_polygon)
{
    // Add piece to jointed_pieces
    jointed_pieces.push_back(jointed_polygon);

    // Replace raw polygon
    polygon = new_frame;

    // Update polygon
    this->updatePolygon(true);
}

void procon::ExpandedPolygon::replaceJointedPieces(std::vector<procon::ExpandedPolygon> pieces)
{
    jointed_pieces = std::move(pieces);
}

// operator
procon::ExpandedPolygon procon::ExpandedPolygon::operator =
(procon::ExpandedPolygon const& p)
{
    this->id = p.id;
    this->polygon = p.polygon;
    this->size = p.size;
    this->inners_size = p.inners_size;
    this->is_inverse = p.is_inverse;
    this->jointed_pieces = p.jointed_pieces;
    this->multi_ids = p.multi_ids;
    this->side_length = p.side_length;
    this->side_angle = p.side_angle;
    this->side_slope = p.side_slope;
    this->inners_side_length = p.inners_side_length;
    this->inners_side_angle = p.inners_side_angle;
    this->inners_side_slope = p.inners_side_slope;
    this->polygon.outer().reserve(32);
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
        //std::cout << "updatePolygonは必要なくなったよ" << std::endl;
    }
}

void procon::ExpandedPolygon::inversePolygon()
{
    if(is_inverse == true){
        is_inverse = false;
    }else{
        is_inverse = true;
    }

    polygon_t translate_polygon;

    boost::geometry::strategy::transform::translate_transformer<double,2,2> transformgo(-centerx,0);
    boost::geometry::transform(polygon,translate_polygon,transformgo);

    polygon_t inversedPolygon;

    for(point_t point : translate_polygon.outer()){
        inversedPolygon.outer().push_back(boost::geometry::model::d2::point_xy<double>(-point.x(),point.y()));
    }

    polygon_t returnPolygon;

    boost::geometry::strategy::transform::translate_transformer<double,2,2> transformback(centerx,0);
    boost::geometry::transform(inversedPolygon,returnPolygon,transformback);

    boost::geometry::reverse(returnPolygon);
    polygon = returnPolygon;
    this->updatePolygon(true);
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

void procon::ExpandedPolygon::sortJointedPieces()
{
    auto lambda = [](procon::ExpandedPolygon const& a,procon::ExpandedPolygon const& b)
    {
        return a.getId() > b.getId();
    };

    std::sort(jointed_pieces.begin(),jointed_pieces.end(),lambda);
}
