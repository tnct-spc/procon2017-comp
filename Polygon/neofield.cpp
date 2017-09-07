#include "neofield.h"
#include "utilities.h"


/*--------------------constructor---------------------*/
procon::NeoField::NeoField()
{
    field_pieces.reserve(50);
    isPlaced.fill(false);
}

/*---------------------public--------------------------*/
//setter
void procon::NeoField::setFrame(std::vector<procon::NeoExpandedPolygon> const& frame)
{
    field_frame = frame;
}

void procon::NeoField::setPiece(procon::NeoExpandedPolygon piece)
{
    field_pieces.push_back(piece);
}

void procon::NeoField::setPiece(NeoExpandedPolygon piece, double x, double y)
{
    piece.setPolygonPosition(x,y);
    field_pieces.push_back(piece);
}

void procon::NeoField::setPiece(procon::NeoExpandedPolygon piece, int n, double x, double y)
{
    piece.setPolygonPosition(x,y);
    field_pieces.at(n) = piece;
}

void procon::NeoField::setElementaryFrame(std::vector<procon::NeoExpandedPolygon> const& frame)
{
    elementary_frame = frame;
}


void procon::NeoField::setElementaryPieces(std::vector<procon::NeoExpandedPolygon> const& pieces)
{
    elementary_pieces = pieces;
    elementary_inverse_pieces.clear();
    for (auto piece : elementary_pieces) {
        piece.inversePolygon();
        elementary_inverse_pieces.emplace_back(piece);
    }
}

void procon::NeoField::setIsPlaced(const std::array<bool,50> &IsPlaced)
{
    isPlaced = IsPlaced;
}

void procon::NeoField::setIsPlaced(int const& piece_id)
{
    isPlaced.at(piece_id) = true;
}

void procon::NeoField::sumTotalEvaluation(const double &eva)
{
    total_evaluation += eva;
}

//getter
std::vector<procon::NeoExpandedPolygon> const& procon::NeoField::getPieces() const
{
    return field_pieces;
}

procon::NeoExpandedPolygon const& procon::NeoField::getPiece(const int &n) const
{
    return field_pieces.at(n);
}

std::vector<procon::NeoExpandedPolygon> const& procon::NeoField::getFrame() const
{
    return field_frame;
}


std::vector<procon::NeoExpandedPolygon> const& procon::NeoField::getElementaryFrame() const
{
    return elementary_frame;
}

std::vector<std::vector<procon::NeoExpandedPolygon>> const& procon::NeoField::getElementaryFrameInnerPices() const
{
    return elementary_frame_inner_pices;
}

std::vector<procon::NeoExpandedPolygon> const& procon::NeoField::getElementaryPieces() const
{
    return elementary_pieces;
}


std::vector<procon::NeoExpandedPolygon> const& procon::NeoField::getElementaryInversePieces() const
{
    return elementary_inverse_pieces;
}

int procon::NeoField::getPiecesSize() const
{
    return static_cast<int>(field_pieces.size());
}

std::array<bool,50> const& procon::NeoField::getIsPlaced() const
{
    return isPlaced;
}

int procon::NeoField::getFieldScore() const
{
    return field_pieces.size();
}

double procon::NeoField::getMinAngle() const
{
    return min_angle;
}

double procon::NeoField::getMinSide() const
{
    return min_side;
}


std::bitset<50> const& procon::NeoField::getPieceID() const
{
    return piece_id;
}

double procon::NeoField::getTotalEvaluation() const
{
    return total_evaluation;
}
//remove
void procon::NeoField::removePiece(int n)
{
    field_pieces.erase(field_pieces.begin() + n);
}

//is_
bool procon::NeoField::isPuttable(procon::NeoExpandedPolygon polygon)
{
//    for (auto inner_ring : field_frame.getPolygon().inners()){
//        if(!boost::geometry::within(polygon.getPolygon(),inner_ring)) return false;
//    }
//    //if(!boost::geometry::within(polygon.getPolygon(), field_frame.getPolygon())) return false;
//    for(auto p : field_pieces){
//        if(!boost::geometry::disjoint(polygon.getPolygon(), p.getPolygon())) return false;
//    }
//    return true;
//    for(auto ex_polygon : field_frame){
//        ex_polygon
//    }
//    TODO: isPuttable判定を実装
    return true;
}

void procon::NeoField::printFrame()
{
    for(auto p : elementary_frame){
        std::cout << bg::dsv(p.getPolygon()) << std::endl;
    }
}

void procon::NeoField::printPiece()
{
    for (auto p : elementary_pieces){
        std::cout << bg::dsv(p.getPolygon()) << std::endl;
    }
}

polygon_i procon::NeoField::translatePolygon(polygon_i polygon, double x, double y)
{
    polygon_i translatedPolygon;
    boost::geometry::strategy::transform::translate_transformer<double,2,2> translate(x,y);
    boost::geometry::transform(polygon,translatedPolygon,translate);
    return translatedPolygon;
}

void procon::NeoField::calcMinAngleSide()
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

void procon::NeoField::calcFieldID()
{
//    for (auto & piece : field_frame.getJointedPieces()) {
//         piece_id.set(piece.getId());
//    }
}
