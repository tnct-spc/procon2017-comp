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
    piece.resetPolygonForcePosition(x,y);
    field_pieces.push_back(piece);
}

void procon::Field::setPiece(procon::ExpandedPolygon piece, int n, double x, double y)
{
    piece.resetPolygonForcePosition(x,y);
    field_pieces.at(n) = piece;
}

void procon::Field::setElementaryFlame(procon::ExpandedPolygon const& flame)
{
    elementary_flame = flame;
}


void procon::Field::setElementaryPieces(std::vector<procon::ExpandedPolygon> const& pieces)
{
    elementary_pieces = pieces;
    elementary_inverse_pieces.clear();
    for (auto piece : elementary_pieces) {
        piece.inversePolygon();
        elementary_inverse_pieces.emplace_back(piece);
    }
}

void procon::Field::setIsPlaced(const std::array<bool,50> &IsPlaced)
{
    isPlaced = IsPlaced;
}

void procon::Field::setIsPlaced(int const& piece_id){
    isPlaced.at(piece_id) = true;
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


std::vector<procon::ExpandedPolygon> const& procon::Field::getElementaryInversePieces() const
{
    return elementary_inverse_pieces;
}

int procon::Field::getPiecesSize() const
{
    return static_cast<int>(field_pieces.size());
}

std::array<bool,50> const& procon::Field::getIsPlaced() const
{
    return isPlaced;
}

int procon::Field::getFieldScore() const
{
    return field_pieces.size();
}

double procon::Field::getMinAngle() const
{
    return min_angle;
}

double procon::Field::getMinSide() const
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

void procon::Field::calcMinAngleSide()
{
    //result buffer
    double min_anglee = 10000;
    double min_sidee = 10000;

    for(auto elementary_piece : elementary_pieces){
        elementary_piece.updatePolygon();

        std::vector<double> angles = elementary_piece.getSideAngle();
        std::vector<double> sides = elementary_piece.getSideLength();

        for(auto angle : angles){
            if(min_anglee > angle){
                min_anglee = angle;
            }
        }
        for(auto side : sides){
            if(min_sidee > side){
                min_sidee = side;
            }
        }
    }

    //set
    min_angle = min_anglee;
    min_side = min_sidee;
}
