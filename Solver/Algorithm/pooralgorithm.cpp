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
    int size;
    int line1;
    int line2;
    field.setFlame(field.getElementaryFlame());
    field.setPiece(field.getElementaryPieces().at(0),15.0,15.0);
    maxpiece = static_cast<int>(field.getElementaryPieces().size());
    for (i=0 ; i<maxpiece ; i++)
    {
        for (j=i+1 ; j<maxpiece ; j++)
        {
            std::vector<std::array<Fit,2>> result;
            status = searchSameLength(field.getElementaryPieces()[i],field.getElementaryPieces()[j], result,line1,line2);
            size = result.size();
            if (status != 0 ){
                std::cout << "Evaluation for Piece" << i << "(" << line1 << ") and Piece" << j <<"(" << line2 << ") ." << std::endl;
                std::cout << "status is " << status << "." << std::endl;
                std::cout << "result size is " << size << "." << std::endl;
            }
        }
    }
    std::cout << "Piece Evaluation Completed..." << std::endl;
    return field;
}

