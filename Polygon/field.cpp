#include "field.h"

/*--------------------constructor---------------------*/
procon::Field::Field()
{
    field_pieces.reserve(50);
    isPlaced.fill(false);
}

/*---------------------public--------------------------*/
//setter
void procon::Field::setFlame(ExpandedPolygon const& flame)
{
    field_flame = flame;
}

void procon::Field::setPiece(procon::ExpandedPolygon piece)
{
    field_pieces.push_back(piece);
}

void procon::Field::setPiece(ExpandedPolygon piece, double x, double y)
{
    polygon_t translated_polygon;
    boost::geometry::strategy::transform::translate_transformer<double,2,2> translate(x,y);
    boost::geometry::transform(piece.getPolygon(),translated_polygon,translate);
    piece.setPolygon(translated_polygon);
    field_pieces.push_back(piece);
}

void procon::Field::setPiece(procon::ExpandedPolygon piece, int n, double x, double y)
{
    polygon_t translated_polygon;
    boost::geometry::strategy::transform::translate_transformer<double,2,2> translate(x,y);
    boost::geometry::transform(piece.getPolygon(),translated_polygon,translate);
    piece.setPolygon(translated_polygon);
    field_pieces.at(n) = piece;
}

void procon::Field::setElementaryFlame(procon::ExpandedPolygon const& flame)
{
    elementary_flame = flame;
}


void procon::Field::setElementaryPieces(std::vector<procon::ExpandedPolygon> const& pieces)
{
    elementary_pieces = pieces;
}

void procon::Field::setIsPlaced(const std::array<bool,50> &IsPlaced)
{
    isPlaced = IsPlaced;
}

//getter
std::vector<procon::ExpandedPolygon> const& procon::Field::getPieces() const
{
    return field_pieces;
}

procon::ExpandedPolygon const& procon::Field::getPiece(const int &n) const
{
    return field_pieces.at(n);
}

procon::ExpandedPolygon const& procon::Field::getFlame() const
{
    return field_flame;
}


procon::ExpandedPolygon const& procon::Field::getElementaryFlame() const
{
    return elementary_flame;
}

std::vector<procon::ExpandedPolygon> const& procon::Field::getElementaryPieces() const
{
    return elementary_pieces;
}

int procon::Field::getPiecesSize()
{
    return static_cast<int>(field_pieces.size());
}

std::array<bool,50> const& procon::Field::getIsPlaced() const
{
    return isPlaced;
}

double procon::Field::getMinAngle()
{
    return min_angle;
}

double procon::Field::getMinSide()
{
    return min_side;
}

//remove
void procon::Field::removePiece(int n)
{
    field_pieces.erase(field_pieces.begin() + n);
}

//is_
bool procon::Field::isPuttable(procon::ExpandedPolygon polygon)
{
    for (auto inner_ring : field_flame.getPolygon().inners()){
        if(!boost::geometry::within(polygon.getPolygon(),inner_ring)) return false;
    }
    //if(!boost::geometry::within(polygon.getPolygon(), field_flame.getPolygon())) return false;
    for(auto p : field_pieces){
        if(!boost::geometry::disjoint(polygon.getPolygon(), p.getPolygon())) return false;
    }
    return true;
}

void procon::Field::printFlame()
{
    std::cout << bg::dsv(elementary_flame.getPolygon()) << std::endl;
}

void procon::Field::printPiece()
{
    for (auto p : elementary_pieces){
        std::cout << bg::dsv(p.getPolygon()) << std::endl;
    }
}

polygon_t procon::Field::translatePolygon(polygon_t polygon, double x, double y)
{
    polygon_t translatedPolygon;
    boost::geometry::strategy::transform::translate_transformer<double,2,2> translate(x,y);
    boost::geometry::transform(polygon,translatedPolygon,translate);
    return translatedPolygon;
}
