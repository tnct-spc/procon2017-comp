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
    std::cout << "status is " << status << "." << std::endl;
    std::cout << "polygon 0 ..." << fit1.start_dot_or_line << ":" << fit1.start_id << " - " << fit1.end_dot_or_line << " : " << fit1.end_id << std::endl;
    std::cout << "polygon "<< i <<" ..." << fit2.start_dot_or_line << ":" << fit2.start_id << " - " << fit2.end_dot_or_line << " : " << fit2.end_id << std::endl;
    }
    return field;
}

