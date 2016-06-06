#include "field.h"

Field::Field()
{
    fieldPiece.resize(50);
    fieldPieceIteratorEnd = fieldPiece.begin();
}

void Field::setFlame(const polygon_t &flame){
    fieldFlame = flame;
}

void Field::pushPiece(const polygon_t &piece){
    *fieldPieceIteratorEnd =  piece;
    fieldPieceIteratorEnd++;
}

polygon_t Field::popPiece(){
    try{
        if (fieldPieceIteratorEnd == fieldPiece.begin()) throw -1;
    }
    catch (int a){
        abort();
    }
    fieldPieceIteratorEnd--;
    polygon_t tmp = *fieldPieceIteratorEnd;
    return tmp;
}

polygon_t Field::getFlame(){
    return fieldFlame;
}

polygon_t Field::getPiece(const int &n){
    return fieldPiece.at(n);
}

void Field::printFlame(){
    std::cout << bg::dsv(fieldFlame) << std::endl;
}

void Field::printPiece(){
    std::for_each(fieldPiece.begin(),fieldPieceIteratorEnd,[](const polygon_t &a){std::cout << bg::dsv(a) << std::endl;});
}
