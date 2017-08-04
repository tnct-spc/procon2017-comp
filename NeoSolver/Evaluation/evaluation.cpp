#include "evaluation.h"
#include "neoexpandedpolygon.h"
#include "Utils/polygonconnector.h"

Evaluation::Evaluation()
{

}

std::vector<std::pair<double , Connect>> Evaluation::evaluation(procon::NeoExpandedPolygon const& frame , procon::NeoExpandedPolygon const& polygon)
{
    auto calculation_nep = [](procon::NeoExpandedPolygon nep , int index , int cal){
        index = index % nep.getSize();
        index = index + cal;
        if(index < 0){
            index = index % nep.getSize();
            index = nep.getSize() + index;
        }
        if(index >= nep.getSize()) index = index % nep.getSize();
        return index;
    };

    //角の状態を返す
    auto angle_status = [&frame , &polygon](int frame_point_index , int polygon_point_index){
        double frame_angle = frame.getSideAngle().at(frame_point_index);
        double polygon_angle = polygon.getSideAngle().at(polygon_point_index);
        //フレームとポリゴンの角がちょうどあっているとき
        if(frame_angle == polygon_angle) return 1;
        //角に隙間があるとき
        else if(frame_angle > polygon_angle) return 0;
        //ポリゴンの角がフレームの角より大きくてありえんとき
        else if(frame_angle < polygon_angle) return -1;

    };

    //辺の状態を返す
    auto length_status = [&frame , &polygon](int frame_side_index , int polygon_side_index){
        double frame_length = frame.getSideLength().at(frame_side_index);
        double polygon_length = polygon.getSideLength().at(polygon_side_index);
        if(frame_length == polygon_length) return 1;
        else return 0;
    };

    std::vector<std::pair<double , Connect>> vector;
    for(int polygon_point_index = 0 ; polygon_point_index < polygon.getSize() ; polygon_point_index++){
        for(int frame_point_index = 0 ; frame_point_index < frame.getSize() ; frame_point_index++){

            bool length_agreement = length_status(frame_point_index , polygon_point_index);
            int angle_agreement = angle_status(frame_point_index , polygon_point_index);
            //辺が同じだったとき
            if(length_agreement && (angle_agreement == 0)){
                int trigger_count = 1;
                angle_agreement = angle_status(calculation_nep(frame , frame_point_index , trigger_count) , calculation_nep(polygon , polygon_point_index , trigger_count));
                length_agreement = length_status(calculation_nep(frame , frame_point_index , trigger_count) , calculation_nep(polygon , polygon_point_index , trigger_count));
                while(angle_agreement && length_agreement){
                    trigger_count++;
                    angle_agreement = angle_status(calculation_nep(frame , frame_point_index , trigger_count) , calculation_nep(polygon , polygon_point_index , trigger_count));
                    length_agreement = length_status(calculation_nep(frame , frame_point_index , trigger_count) , calculation_nep(polygon , polygon_point_index , trigger_count));
                }
                double evaluation = std::pow(trigger_count , 2.0);
                Connect connect;
                connect.frame_side_index = frame_point_index;
                connect.polygon_side_index = polygon_point_index;
                connect.frame_point_index = frame_point_index;
                connect.polygon_point_index = polygon_point_index;
                vector.push_back(std::pair<double , Connect>(evaluation , connect));
            //角が同じだったとき
            }else if((!length_agreement) && (angle_agreement == 1)){
                double evaluation = 1;
                Connect connect;
                connect.frame_side_index = calculation_nep(frame , frame_point_index , -1);
                connect.polygon_side_index = calculation_nep(polygon , polygon_point_index , -1);
                connect.frame_point_index = frame_point_index;
                connect.polygon_point_index = polygon_point_index;
                vector.push_back((std::pair<double , Connect>(evaluation , connect)));
            }
        }
    }

    return vector;
}
