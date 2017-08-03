#include "evaluation.h"
#include "neoexpandedpolygon.h"

Evaluation::Evaluation()
{

}

typedef std::tuple<int , int , int , int ,int> my_tuple;

std::vector<my_tuple> Evaluation::evaluation(procon::NeoExpandedPolygon const& frame , procon::NeoExpandedPolygon const& polygon)
{    
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
    auto gap_evaluation = [&minus_one , &length , &frame , &polygon](int frame_point_index , int polygon_point_index){

        int frame_side_index1 = minus_one(frame , frame_point_index);
        int frame_side_index2 = frame_point_index;
        int polygon_side_index1 = minus_one(polygon , polygon_point_index);
        int polygon_side_index2 = polygon_point_index;

        //それぞれ評価値 , frame_side_index , polygon_side_index , frame_point_index , polygon_point_index
        std::vector<my_tuple> vector;
        vector.push_back(
                    my_tuple(
                        length(frame_side_index1 , polygon_side_index1),
                        frame_side_index1,
                        polygon_side_index1,
                        frame_point_index,
                        polygon_point_index
                    )
        );
        vector.push_back(
                    my_tuple(
                        length(frame_side_index2 , polygon_side_index2),
                        frame_side_index2,
                        polygon_side_index2,
                        frame_point_index,
                        polygon_point_index
                    )
        );
        return vector;
    };

    int frame_point_index , polygon_point_index;
    std::vector<my_tuple> vector;
    for(frame_point_index = 0 ; frame_point_index < frame.getSize() ; frame_point_index++){
        for(polygon_point_index = 0 ; polygon_point_index < polygon.getSize() ; polygon_point_index++){
            if(angle_status(frame_point_index , polygon_point_index) == 1){
                //フレームとポリゴンの角がちょうどあっているとき
                int frame_side_index = minus_one(frame , frame_point_index);
                int point_side_index = minus_one(polygon , polygon_point_index);
                vector.push_back(my_tuple(length(frame_side_index , point_side_index)
                                          + length(frame_point_index , polygon_point_index),
                                          minus_one(frame , frame_point_index),
                                          minus_one(polygon , polygon_point_index),
                                          frame_point_index,
                                          polygon_point_index
                                          )
                                 );
            }else if(angle_status(frame_point_index , polygon_point_index) == 0){
                //角に隙間があるとき
                std::vector<my_tuple> v = gap_evaluation(frame_point_index , polygon_point_index);
                vector.insert(vector.end() , v.begin() , v.end());
            }else if(angle_status(frame_point_index , polygon_point_index) == -1){
                //ポリゴンの角がフレームの角より大きくてありえんとき
                vector.push_back(my_tuple(-1,
                                          minus_one(frame , frame_point_index),
                                          minus_one(polygon , polygon_point_index),
                                          frame_point_index,
                                          polygon_point_index
                                          )
                                 );
            }
        }
    }

    return vector;
}
