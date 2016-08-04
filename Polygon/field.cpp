#include "field.h"
/*--------------------constructor---------------------*/
procon::Field::Field()
{
    field_pieces.reserve(50);
}

/*---------------------public--------------------------*/
//setter
void procon::Field::setFlame(const ExpandedPolygon &flame)
{
    field_flame = flame;
}

void procon::Field::setPiece(ExpandedPolygon piece, double x, double y)
{
    polygon_t translated_polygon;
    boost::geometry::strategy::transform::translate_transformer<double,2,2> translate(x,y);
    boost::geometry::transform(piece.getPolygon(),translated_polygon,translate);
    ExpandedPolygon p;
    p.setPolygon(translated_polygon);
    field_pieces.push_back(p);
}

void procon::Field::setPiece(procon::ExpandedPolygon piece, int n, double x, double y)
{
    polygon_t translated_polygon;
    boost::geometry::strategy::transform::translate_transformer<double,2,2> translate(x,y);
    boost::geometry::transform(piece.getPolygon(),translated_polygon,translate);
    ExpandedPolygon p;
    p.setPolygon(translated_polygon);
    field_pieces.at(n) = p;
}

//getter
std::vector<procon::ExpandedPolygon> procon::Field::getPieces()
{
    return field_pieces;
}

procon::ExpandedPolygon procon::Field::getPiece(const int &n)
{
    return field_pieces.at(n);
}

procon::ExpandedPolygon procon::Field::getFlame()
{
    return field_flame;
}

int procon::Field::getPiecesSize()
{
    return static_cast<int>(field_pieces.size());
}

//remove
void procon::Field::removePiece(int n)
{
    field_pieces.erase(field_pieces.begin() + n);
}

bool procon::Field::isPuttable(procon::ExpandedPolygon polygon)
{
    if(!boost::geometry::within(polygon.getPolygon(), field_flame.getPolygon())) return false;
    for(auto p : field_pieces){
        if(!boost::geometry::disjoint(polygon.getPolygon(), p.getPolygon())) return false;
    }
    return true;
}

void procon::Field::printFlame()
{
    std::cout << bg::dsv(field_flame.getPolygon()) << std::endl;
}

void procon::Field::printPiece()
{
    for (auto p : field_pieces){
        std::cout << bg::dsv(p.getPolygon()) << std::endl;
    }
}
