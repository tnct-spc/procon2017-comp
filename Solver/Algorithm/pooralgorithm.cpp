#include "pooralgorithm.h"

PoorAlgorithm::PoorAlgorithm()
{

}

procon::Field PoorAlgorithm::run(procon::Field field)
{
    field.setFlame(field.getElementaryFlame());
    field.setPiece(field.getElementaryPieces().at(0),15.0,15.0);
    const int maxpiece = static_cast<int>(field.getElementaryPieces().size());
    for (int i = 0 ; i < maxpiece ; i++) {
        for (int j = i + 1 ; j<maxpiece ; j++) {
            std::vector<std::array<Fit,2>> result;
            auto piece1 = field.getElementaryPieces().at(i);
            auto piece2 = field.getElementaryPieces().at(j);
            const int status = searchSameLength(piece1,piece2, result);
            const int size = result.size();
            if (status != 0 ){
                std::cout << "Evaluation for Piece" << i << "(" << piece1.getSize() << ") and Piece" << j <<"(" << piece2.getSize() << ") ." << std::endl;
                std::cout << "status is " << status << "." << std::endl;
                std::cout << "result size is " << size << "." << std::endl;
            }
        }
    }
    std::cout << "Piece Evaluation Completed..." << std::endl;
    return field;
}

