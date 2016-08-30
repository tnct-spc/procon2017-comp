#include "pooralgorithm.h"

PoorAlgorithm::PoorAlgorithm()
{

}

procon::Field PoorAlgorithm::run(procon::Field field)
{
    int status;
    int i;
    int j;
    int maxpiece;
    std::vector<std::array<Fit,2>> result;
    field.setFlame(field.getElementaryFlame());
    field.setPiece(field.getElementaryPieces().at(0),15.0,15.0);
    maxpiece = static_cast<int>(field.getElementaryPieces().size());
    for (i=0 ; i<maxpiece ; i++)
    {
        for (j=0 ; j<maxpiece ; j++)
        {
            status = searchSameLength(field.getElementaryPieces()[i],field.getElementaryPieces()[j], result);
            std::cout << "status is " << status << "." << std::endl;
            std::cout << "result size is " << result.size() << "." << std::endl;
        }
    }
    return field;
}

