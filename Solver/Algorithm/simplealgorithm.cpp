#include "simplealgorithm.h"

SimpleAlgorithm::SimpleAlgorithm()
{

}

procon::Field SimpleAlgorithm::run(procon::Field field)
{
    field.setFlame(field.getElementaryFlame());
    for(auto elementary_piece : field.getElementaryPieces()){
        for(double x = 0;x < 30;x++){
            for(double y = 0;y < 30;y++){                   
                elementary_piece.setPolygonPosition(x,y);
                for(int degree = 0; degree < 360; degree = degree + 10){
                    elementary_piece.setPolygonAngle(degree);
                    if(field.isPuttable(elementary_piece)){
                        field.setPiece(elementary_piece);
                        goto next_roop;
                    }
                }
            }
        }
        next_roop:;
    }
    for (auto piece : field.getPieces()){
        piece.updatePolygon();
        std::cout << "hoge" << std::endl;
    }
    return field;
}
