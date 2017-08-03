#include "evaluation.h"
#include "neoexpandedpolygon.h"
#include "Utils/polygonconnector.h"

Evaluation::Evaluation()
{

}

std::vector<std::pair<double , Connect>> Evaluation::evaluation(procon::NeoExpandedPolygon const& frame , procon::NeoExpandedPolygon const& polygon)
{    
    double const angle_weight = 0.9;
    double const length_weight = 1.1;

    //NEPの要素数をマイナス1するだけのらむだ
    auto minus_one = [](procon::NeoExpandedPolygon nep , int index){
        index =index - 1;
        if(index == -1) index = nep.getSize() - 1;
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
    auto length = [&frame , &polygon](int frame_side_index , int polygon_side_index){
        double frame_length = frame.getSideLength().at(frame_side_index);
        double polygon_length = polygon.getSideLength().at(polygon_side_index);
        if(frame_length == polygon_length) return 1;
        else return 0;
    };

    //隙間があるとき評価値の集まりを返す
    auto gap_evaluation = [&length_weight , &minus_one , &length , &frame , &polygon]
            (int frame_point_index , int polygon_point_index){

        int frame_side_index1 = minus_one(frame , frame_point_index);
        int frame_side_index2 = frame_point_index;
        int polygon_side_index1 = minus_one(polygon , polygon_point_index);
        int polygon_side_index2 = polygon_point_index;

        //それぞれ評価値 , frame_side_index , polygon_side_index , frame_point_index , polygon_point_index
        std::vector<std::pair<double , Connect>> vector;
        double evaluation = length(frame_side_index1 , polygon_side_index1) * length_weight;
        Connect connect;
        connect.frame_side_index = frame_side_index1;
        connect.polygon_side_index = polygon_side_index1;
        connect.frame_point_index = frame_point_index;
        connect.polygon_point_index = polygon_point_index;

        vector.push_back(std::pair<double , Connect>(evaluation , connect));

        evaluation = length(frame_side_index2 , polygon_side_index2) * length_weight;
        connect.frame_side_index = frame_side_index2;
        connect.polygon_side_index = polygon_side_index2;
        connect.frame_point_index = frame_point_index;
        connect.polygon_point_index = polygon_point_index;

        vector.push_back(std::pair<double , Connect>(evaluation , connect));
        return vector;
    };

    //角があったときとありえんときの処理
    auto default_evaluation = [&angle_weight , &length_weight , &minus_one , &length , &frame , &polygon]
            (int angle_evaluation , int frame_point_index , int polygon_point_index){
        int frame_side_index = minus_one(frame , frame_point_index);
        int polygon_side_index = minus_one(polygon , polygon_point_index);

        int length_evaluation;
        double evaluation;
        if(angle_evaluation == -1){
            //ありえんときの評価は問答無用で-1
            evaluation = -1;
        }else{
            //角がぴったりだったときの辺の長さの評価
            length_evaluation =
                    length(frame_side_index , polygon_side_index)
                    +
                    length(frame_point_index , polygon_point_index);
                    evaluation = angle_evaluation * angle_weight + length_evaluation * length_weight;
        }

        Connect connect;
        connect.frame_side_index = frame_side_index;
        connect.polygon_side_index = polygon_side_index;
        connect.frame_point_index = frame_point_index;
        connect.polygon_point_index = polygon_point_index;

        return std::pair<double , Connect>(evaluation , connect);
    };

    int frame_point_index , polygon_point_index;
    std::vector<std::pair<double , Connect>> vector;
    for(frame_point_index = 0 ; frame_point_index < frame.getSize() ; frame_point_index++){
        for(polygon_point_index = 0 ; polygon_point_index < polygon.getSize() ; polygon_point_index++){
            int angle_evaluation = angle_status(frame_point_index , polygon_point_index);
            if (angle_evaluation == 0){
                //角に隙間があるとき
                std::vector<std::pair<double , Connect>> v = gap_evaluation(frame_point_index , polygon_point_index);
                vector.insert(vector.end() , v.begin() , v.end());
            } else {
                //フレームとポリゴンの角がちょうどあっているとき
                //ポリゴンの角がフレームの角より大きくてありえんとき
                vector.push_back(default_evaluation(angle_evaluation , frame_point_index , polygon_point_index));
            }
        }
    }

    return vector;
}
