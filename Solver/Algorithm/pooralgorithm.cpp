#include "pooralgorithm.h"

PoorAlgorithm::PoorAlgorithm()
{

}

procon::Field PoorAlgorithm::run(procon::Field field)
{
    int status;
    std::vector<std::array<Fit,2>> result;
    field.setFlame(field.getElementaryFlame());
    field.setPiece(field.getElementaryPieces().at(0),15.0,15.0);
    for (int i=1 ; i<21 ; i++)
    {
        status = searchSameLength(field.getElementaryPieces()[0],field.getElementaryPieces()[i], result);
        std::cout << "status is " << status << "." << std::endl;
    }
    return field;
}

