#include "field.h"

Field::Field()
{
    fieldPiece.reserve(50);
}
void Field::setFlame(const PolygonExpansion &flame){
    fieldFlame = flame;
}

void Field::setPiece(const PolygonExpansion &piece,const int &n, double x, double y){
    if (n < pieceSize()) fieldPiece[n] = piece;
    else pushPiece(piece, x, y);
}

void Field::pushPiece(const PolygonExpansion &piece, double x, double y){
    polygon_t translated_polygon;
    PolygonExpansion piece_buff = piece;
    boost::geometry::strategy::transform::translate_transformer<double,2,2> translate(x,y);
    boost::geometry::transform(piece_buff.getPolygon(),translated_polygon,translate);
    piece_buff.setPolygon(translated_polygon);
    fieldPiece.push_back(piece_buff);
}

PolygonExpansion Field::popPiece(){
    PolygonExpansion tmp = fieldPiece.back();
    fieldPiece.pop_back();
    return tmp;
}

PolygonExpansion Field::getPiece(const int &n){
    return fieldPiece[n];
}

PolygonExpansion Field::getFlame(){
    return fieldFlame;
}


/*後方互換*/
/*
void Field::setFlame(const polygon_t &flame){
    fieldFlame.setPolygon(flame);
}

void Field::setPiece(const polygon_t &piece,const int &n){
    if (n < pieceSize()) fieldPiece[n].setPolygon(piece);
    else pushPiece(piece);
}

void Field::pushPiece(const polygon_t &piece){
    PolygonExpansion tmp;
    tmp.setPolygon(piece);
    fieldPiece.push_back(tmp);
}

polygon_t Field::popPiece(){
    PolygonExpansion tmp = fieldPiece.back();
    fieldPiece.pop_back();
    return tmp.getPolygon();
}

polygon_t Field::getFlame() {
    return fieldFlame.getPolygon();
}

polygon_t Field::getPiece(const int &n) {
    return fieldPiece[n].getPolygon();
}
*/
/*ここまで*/

int Field::pieceSize(){
    return (int)(fieldPiece.end() - fieldPiece.begin());
}

void Field::printFlame(){
    std::cout << bg::dsv(fieldFlame.getPolygon()) << std::endl;
}

void Field::printPiece(){
    std::for_each(fieldPiece.begin(),fieldPiece.end(),[](PolygonExpansion &a){std::cout << bg::dsv(a.getPolygon()) << std::endl;});
}
