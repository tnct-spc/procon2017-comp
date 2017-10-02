#include "neofield.h"
#include "utilities.h"
#include "neopolygonviewer.h"
#include <mutex>

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

void procon::NeoField::setElementaryFrameInnerPieces(std::vector<procon::NeoExpandedPolygon> const& frame_inner_pices)
{
    elementary_frame_inner_pices = frame_inner_pices;
}

void procon::NeoField::setIsPlaced(const std::array<bool,50> &IsPlaced)
{
    isPlaced = IsPlaced;
}

void procon::NeoField::setIsPlaced(int const& piece_id)
{
    isPlaced.at(piece_id) = true;
}

void procon::NeoField::setIsPlaced(const int &piece_id, bool is_placed)
{
    isPlaced.at(piece_id) = is_placed;
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

std::vector<procon::NeoExpandedPolygon> const& procon::NeoField::getElementaryFrameInnerPices() const
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

//#define test
//#define check_conflict

bool procon::NeoField::check_hint()
{
    std::mutex mtx;
#ifdef test
    std::cout << "start check_hint!!" << std::endl;
#endif

    std::vector<procon::NeoExpandedPolygon> hint_pieces = elementary_frame_inner_pices;

    unsigned int hint_index = 0;
    for(const procon::NeoExpandedPolygon &hint_piece : hint_pieces) {
        if(isPlaced.at(static_cast<unsigned int>(hint_piece.getId()))) hint_pieces.erase(hint_pieces.begin() + hint_piece.getId());
        ++hint_index;
    }

    field_frame.reserve(100);
    field_pieces.reserve(100);


    std::function<bool(std::vector<procon::NeoExpandedPolygon>)> check_hint_and_connect =
            [&check_hint_and_connect, &mtx, this](std::vector<procon::NeoExpandedPolygon> hint_pieces)
    {
#ifdef test
        auto out = [](polygon_i poly, std::string name)
        {
            std::cout << name << " -> ";

            for(point_i point : poly.outer()) {
                std::cout << "(" << point.x() << ", " << point.y() << "), ";
            }

            std::cout << std::endl;
        };

        int doing = 0;
#endif

        //180度角除去
        std::function<void(polygon_i&)> delete_180_degree = [&delete_180_degree](polygon_i &poly)
        {
            procon::NeoExpandedPolygon expoly;
            expoly.resetPolygonForce(poly);
            std::vector<double> angles = expoly.getSideAngle();
            unsigned int i = 0;
            for(double angle : angles) {
                if(!(angle < M_PI || angle > M_PI)) {
                    std::vector<point_i> &outer = poly.outer();
                    outer.erase(outer.begin() + i);
                    if(i == 0) outer.at(outer.size() - 1) = outer.at(0);
                    delete_180_degree(poly);
                    break;
                }
                ++i;
            }
        };

        bool check_all_float = true; //ヒントが全部浮いてるか
            unsigned int hint_index = 0;
            for(const procon::NeoExpandedPolygon &hint_piece : hint_pieces) {
                if(!isPlaced.at(static_cast<unsigned int>(hint_piece.getId()))) {
                    std::vector<procon::NeoExpandedPolygon> frames_next;
                    frames_next.reserve(100);
#ifdef test
                    std::cout << std::endl << "now hint is " << hint_index << " --------------" << std::endl;

                    out(hint_piece.getPolygon(), "hint");
    //                NeoPolygonViewer::getInstance().displayPolygon(hint_piece.getPolygon(), "do " + std::to_string(doing) + "hint " + std::to_string(hint_index) + " hint_piece", false);

                    int ii = 0;
                    for(const procon::NeoExpandedPolygon &frame : field_frame) {
                        if(!frame.getPolygon().outer().empty()) {
                            out(frame.getPolygon(), "frame " + std::to_string(ii));
    //                        NeoPolygonViewer::getInstance().displayPolygon(frame.getPolygon(), "do " + std::to_string(doing) + "hint " + std::to_string(hint_index) + " frame " + std::to_string(ii), false);
                        } else std::cout << "frame " << ii << " is empty" << std::endl;
                        ++ii;
                    }
                    int i = 0;
#endif
                    bool check_float = false; //ヒントが浮いてるか
                    for(const procon::NeoExpandedPolygon &frame : field_frame) {
                        std::vector<polygon_i> union_polygons;
                        bg::union_(frame.getPolygon(), hint_piece.getPolygon(), union_polygons);
                        if(union_polygons.size() == 1) { //ヒントがこのフレームにある！
#ifdef test
                            std::cout << "hint in frame " << i << " !" << std::endl;
                            int iii = 0;
                            for(const polygon_i &union_polygon : union_polygons) {
                                if(!union_polygon.outer().empty()) {
                                    out(union_polygon, "union " + std::to_string(iii));
    //                                NeoPolygonViewer::getInstance().displayPolygon(union_polygon, "do " + std::to_string(doing) + "hint " + std::to_string(hint_index) + " union " + std::to_string(iii), false);
                                } else std::cout << "union " << ii << " is empty" << std::endl;
                                ++iii;
                            }
#endif
                            if(!bg::equals(frame.getPolygon(), union_polygons.at(0))) { //ヒントがコンフリクトしてる
#ifdef test
                                std::cout << "hint conflicting" << std::endl;
#endif
                                {
#ifdef check_conflict
                                    std::lock_guard<decltype(mtx)> lock(mtx);
                                    std::string log;
                                    std::cout << "-------------Conflicting!!!!---------------------" << std::endl;
                                    log = "frame -> ";
                                    for(point_i point : frame.getPolygon().outer()) {
                                        log += "(" + std::to_string(point.x()) + ", " + std::to_string(point.y()) + "), ";
                                    }
                                    std::cout << log << std::endl;

                                    log = "hint -> ";
                                    for(point_i point : hint_piece.getPolygon().outer()) {
                                        log += "(" + std::to_string(point.x()) + ", " + std::to_string(point.y()) + "), ";
                                    }
                                    std::cout << log << std::endl;
                                    std::cout << "-------------------------------------------------" << std::endl;
#endif
                                    return false;
                                }
                            }
                            std::vector<polygon_i> frames_new;
                            bg::difference(frame.getPolygon(), hint_piece.getPolygon(), frames_new);
                            for(polygon_i &frame_new : frames_new) {
                                delete_180_degree(frame_new);
                            }
                            for(const polygon_i &frame_new : frames_new) {
                                if(frame_new.inners().empty()) { //ちゃんとヒントがはまった
#ifdef test
                                std::cout << "set hint!" << std::endl;
#endif
                                    procon::NeoExpandedPolygon frame_next;
                                    frame_next.resetPolygonForce(frame_new);
                                    frames_next.emplace_back(frame_next);
                                    check_all_float = false;
                                } else { //ヒントが浮いてる
#ifdef test
                                std::cout << "not set hint. hint froating." << std::endl;
#endif
                                    check_float = true;
                                    break;
                                }
                            }
                        } else frames_next.emplace_back(frame); //ヒントがこのフレームになかったらスルー
                        if(check_float) break;
#ifdef test
                        ++i;
#endif
                    }
                    if(!check_float) {
                        field_frame = frames_next;
                        field_pieces.emplace_back(hint_piece);
                        isPlaced.at(static_cast<unsigned int>(hint_piece.getId())) = true;
                    }
                }
                ++hint_index;
            }
#ifdef test
            ++doing;
#endif
        if(!check_all_float) { //ヒントがはまらなくなるまで再帰
            bool check = check_hint_and_connect(hint_pieces);
            return (check) ? true : false;
        } else return true;
    };

    bool check = check_hint_and_connect(hint_pieces);
    return (check) ? true : false;
}

