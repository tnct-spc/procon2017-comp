#include "pooralgorithm.h"

PoorAlgorithm::PoorAlgorithm()
{

}

procon::Field PoorAlgorithm::run(procon::Field field)
{
    int status;
    Fit fit1;
    Fit fit2;
    field.setFlame(field.getElementaryFlame());
    field.setPiece(field.getElementaryPieces().at(0),15.0,15.0);
    for (int i=1 ; i<21 ; i++)
    {
    status = searchSameLength(field.getElementaryPieces()[0],field.getElementaryPieces()[i], fit1, fit2);
    }
    return field;
}

