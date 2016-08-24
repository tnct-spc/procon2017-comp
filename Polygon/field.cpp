#include "field.h"
/*--------------------constructor---------------------*/
procon::Field::Field()
{
    field_pieces.reserve(50);
}

/*---------------------public--------------------------*/
//setter
void procon::Field::setFlame(ExpandedPolygon const& flame)
{
    field_flame = flame;
}

void procon::Field::setPiece(polygon_t piece)
{
    procon::ExpandedPolygon ex_piece;
    ex_piece.setPolygon(piece);
    field_pieces.push_back(ex_piece);
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

void procon::Field::setElementaryFlame(procon::ExpandedPolygon const& flame)
{
    elementary_flame = flame;
}


void procon::Field::setElementaryPieces(std::vector<procon::ExpandedPolygon> const& pieces)
{
    elementary_pieces = pieces;
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
//remove
void procon::Field::removePiece(int n)
{
    field_pieces.erase(field_pieces.begin() + n);
}

//is_
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
    std::cout << bg::dsv(elementary_flame.getPolygon()) << std::endl;
}

void procon::Field::printPiece()
{
    for (auto p : field_pieces){
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
