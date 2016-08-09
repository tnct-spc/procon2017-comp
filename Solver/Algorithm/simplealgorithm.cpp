#include "simplealgorithm.h"

SimpleAlgorithm::SimpleAlgorithm()
{

}

procon::Field SimpleAlgorithm::run(procon::Field field)
{
    procon::ExpandedPolygon ExP_buff;
    polygon_t P_buff;

    field.setFlame(field.getElementaryFlame());
    int size = field.getElementaryPieces().size();
    for(int i=0;i<size;i++){
        for(double x=0;x<30;x++){
            for(double y=0;y<30;y++){
                boost::geometry::strategy::transform::translate_transformer<double,2,2> translate(x,y);
                boost::geometry::transform(field.getElementaryPieces().at(i).getPolygon(),P_buff,translate);
                ExP_buff.setPolygon(P_buff);
                if(field.isPuttable(ExP_buff)){
                    field.setPiece(ExP_buff);
                    goto next;
                }
            }
        }
        next:;
    }
    return field;
}
