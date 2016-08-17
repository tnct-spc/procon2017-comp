#include "simplealgorithm.h"

SimpleAlgorithm::SimpleAlgorithm()
{

}

procon::Field SimpleAlgorithm::run(procon::Field field)
{
    polygon_t P_buff;

    field.setFlame(field.getElementaryFlame());
    for(auto elementary_piece : field.getElementaryPieces()){
        for(double x = 0;x < 30;x++){
            for(double y = 0;y < 30;y++){
                procon::ExpandedPolygon piece = elementary_piece;
                boost::geometry::strategy::transform::translate_transformer<double,2,2> translate(x,y);
                boost::geometry::transform(piece.getPolygon(),P_buff,translate);
                piece.setPolygon(P_buff);
                if(field.isPuttable(piece)){
                    field.setPiece(piece);
                    goto next_roop;
                }
            }
        }
        next_roop:;
    }

    return field;
}
