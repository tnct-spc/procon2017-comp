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
void procon::Field::setFrame(NeoExpandedPolygon const& frame)
{
    field_frame = frame;
}

void procon::Field::setPiece(procon::NeoExpandedPolygon piece)
{
    field_pieces.push_back(piece);
}

void procon::Field::setPiece(NeoExpandedPolygon piece, double x, double y)
{
    piece.setPolygonPosition(x,y);
    field_pieces.push_back(piece);
}

void procon::Field::setPiece(procon::NeoExpandedPolygon piece, int n, double x, double y)
{
    piece.setPolygonPosition(x,y);
    field_pieces.at(n) = piece;
}

void procon::Field::setElementaryFrame(procon::NeoExpandedPolygon const& frame)
{
    elementary_frame = frame;
}


void procon::Field::setElementaryPieces(std::vector<procon::NeoExpandedPolygon> const& pieces)
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

void procon::Field::setIsPlaced(int const& piece_id)
{
    isPlaced.at(piece_id) = true;
}

void procon::Field::sumTotalEvaluation(const double &eva)
{
    total_evaluation += eva;
}

//getter
std::vector<procon::NeoExpandedPolygon> const& procon::Field::getPieces() const
{
    return field_pieces;
}

procon::NeoExpandedPolygon const& procon::Field::getPiece(const int &n) const
{
    return field_pieces.at(n);
}

procon::NeoExpandedPolygon const& procon::Field::getFrame() const
{
    return field_frame;
}


procon::NeoExpandedPolygon const& procon::Field::getElementaryFrame() const
{
    return elementary_frame;
}


std::vector<procon::NeoExpandedPolygon> const& procon::Field::getElementaryPieces() const
{
    return elementary_pieces;
}


std::vector<procon::NeoExpandedPolygon> const& procon::Field::getElementaryInversePieces() const
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


std::bitset<50> const& procon::Field::getPieceID() const
{
    return piece_id;
}

double procon::Field::getTotalEvaluation() const
{
    return total_evaluation;
}
//remove
void procon::Field::removePiece(int n)
{
    field_pieces.erase(field_pieces.begin() + n);
}

//is_
bool procon::Field::isPuttable(procon::NeoExpandedPolygon polygon)
{
    for (auto inner_ring : field_frame.getPolygon().inners()){
        if(!boost::geometry::within(polygon.getPolygon(),inner_ring)) return false;
    }
    //if(!boost::geometry::within(polygon.getPolygon(), field_frame.getPolygon())) return false;
    for(auto p : field_pieces){
        if(!boost::geometry::disjoint(polygon.getPolygon(), p.getPolygon())) return false;
    }
    return true;
}

void procon::Field::printFrame()
{
    std::cout << bg::dsv(elementary_frame.getPolygon()) << std::endl;
}

void procon::Field::printPiece()
{
    for (auto p : elementary_pieces){
        std::cout << bg::dsv(p.getPolygon()) << std::endl;
    }
}

polygon_i procon::Field::translatePolygon(polygon_i polygon, double x, double y)
{
    polygon_i translatedPolygon;
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

bool procon::Field::operator || (Field & field) {
    if (this->piece_id != field.piece_id) return false;

    //ソート（これによって比較が可能に)
    this->field_frame.sortJointedPieces();
    field.field_frame.sortJointedPieces();

    for (unsigned int i = 0;i < this->getFrame().getJointedPieces().size();i++) {

        //slope check
        {
            double const& slope_a = this->getFrame().getJointedPieces().at(i).getSideSlope().at(0);
            double const& slope_b = field.getFrame().getJointedPieces().at(i).getSideSlope().at(0);
            constexpr double threshold = 3.141592653589 / 180;
            if (!Utilities::nearlyEqual(slope_a,slope_b,threshold)) return false;
        }
        //coord test
        {
            point_i const& coord_a = this->getFrame().getJointedPieces().at(i).getPolygon().outer().at(0);
            point_i const& coord_b = field.getFrame().getJointedPieces().at(i).getPolygon().outer().at(0);
            constexpr double threshold = 0.5;
            if (!Utilities::nearlyEqual(coord_a.x(),coord_b.x(),threshold)) return false;
            if (!Utilities::nearlyEqual(coord_a.y(),coord_b.y(),threshold)) return false;
        }
    }
    return true;
}

void procon::Field::calcFieldID()
{
    for (auto & piece : field_frame.getJointedPieces()) {
         piece_id.set(piece.getId());
    }
}
