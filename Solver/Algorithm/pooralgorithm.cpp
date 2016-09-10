#include "pooralgorithm.h"

PoorAlgorithm::PoorAlgorithm()
{

}

procon::Field PoorAlgorithm::run(procon::Field field)
{
    field.setFlame(field.getElementaryFlame());
    auto hoge = field.getElementaryPieces();
    field.setPiece(field.getElementaryPieces().at(0),15.0,15.0);
    return field;

}

