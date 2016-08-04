#include "simplealgorithm.h"

SimpleAlgorithm::SimpleAlgorithm()
{

}

procon::Field SimpleAlgorithm::run(PolygonSet set)
{
    procon::Field field;
    procon::ExpandedPolygon ExP_buff;
    polygon_t P_buff;

    field.setFlame(set.fieldFlame);
    int size = set.fieldPiece.size();
    for(int i=0;i<size;i++){
        for(double x=0;x<30;x++){
            for(double y=0;y<30;y++){
                boost::geometry::strategy::transform::translate_transformer<double,2,2> translate(x,y);
                boost::geometry::transform(set.fieldPiece.at(i).getPolygon(),P_buff,translate);
                ExP_buff = set.fieldPiece.at(i);
                ExP_buff.setPolygon(P_buff);
                if(field.isPuttable(ExP_buff)){
                    field.pushPiece(ExP_buff);
                    goto next;
                }
            }
        }
        next:;
    }

    return field;
}
