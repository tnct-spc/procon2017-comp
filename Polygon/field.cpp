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
    PolygonExpansion old_piece = piece;
    PolygonExpansion new_piece;
    polygon_t polygon;
    int size = piece.size();
    for(int i=0;i<size;i++){
        polygon.outer().push_back(point_t(old_piece.getPolygon().outer()[i].x()+x,old_piece.getPolygon().outer()[i].y()+y));
    }
    new_piece.setPolygon(polygon);
    fieldPiece.push_back(new_piece);
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
