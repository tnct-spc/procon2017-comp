#include "pooralgorithm.h"

PoorAlgorithm::PoorAlgorithm()
{

}

procon::Field PoorAlgorithm::run(procon::Field field)
{
    int status;
    int re1;
    int re2;
    int re3;
    int re4;
    double re5;
    double re6;
    double re7;
    double re8;
    double re9;
    double re10;
    field.setFlame(field.getElementaryFlame());
    field.setPiece(field.getElementaryPieces().at(0),15.0,15.0);
    for (int i=1 ; i<21 ; i++){
    status = searchSameLength(field.getElementaryPieces()[0],field.getElementaryPieces()[i], re1, re2, re3, re4, re5, re6, re7, re8, re9, re10);
    std::cout << "status ="<< status << "  " << "0&" <<i << " = " << re1 << "-" << re2 << ":" << re3 << "-" << re4 << "(" << re5 << ":" << re6 << ":" << re7 << ":" << re8 << ":" << re9 << ":" << re10 << ")" << std::endl ;
    }
    return field;
}
