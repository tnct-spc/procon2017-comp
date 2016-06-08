#include "field.h"

Field::Field()
{
    fieldPiece.reserve(50);
}

void Field::setFlame(const polygon_t &flame){
    fieldFlame = flame;
}

void Field::setPiece(const polygon_t &piece,const int &n){
    fieldPiece.at(n) = piece;
}

void Field::pushPiece(const polygon_t &piece){
    fieldPiece.push_back(piece);
}

polygon_t Field::popPiece(){
    polygon_t tmp = fieldPiece.back();
    fieldPiece.pop_back();
    return tmp;
}

polygon_t Field::getFlame() const{
    return fieldFlame;
}

polygon_t Field::getPiece(const int &n) const{
    return fieldPiece.at(n);
}

void Field::printFlame(){
    std::cout << bg::dsv(fieldFlame) << std::endl;
}

void Field::printPiece(){
    std::for_each(fieldPiece.begin(),fieldPiece.end(),[](const polygon_t &a){std::cout << bg::dsv(a) << std::endl;});
}
