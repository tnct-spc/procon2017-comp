#include "simplealgorithm.h"

#include "singlepolygondisplay.h"

SimpleAlgorithm::SimpleAlgorithm()
{

}

void SimpleAlgorithm::run(procon::Field field)
{
    field.setFrame(field.getElementaryFrame());
    for(auto elementary_piece : field.getElementaryPieces()){
        for(double x = 0;x < 30;x++){
            for(double y = 0;y < 30;y++){
                elementary_piece.setPolygonPosition(x,y);
                for(int degree = 0; degree < 360; degree = degree + 10){
                    elementary_piece.setPolygonAngle(degree);
                    elementary_piece.inversePolygon();
                    if(field.isPuttable(elementary_piece)){
                        field.setPiece(elementary_piece);
                        goto next_roop;
                    }
                }
            }
        }
        next_roop:;
    }
    submitAnswer(field);
    return;
}
