#include "evaluation.h"
#include "neoexpandedpolygon.h"

Evaluation::Evaluation()
{

}


std::vector<std::tuple<int , int , int , int , int>> Evaluation::evaluation(procon::NeoExpandedPolygon const& frame , procon::NeoExpandedPolygon const& polygon)
{    
    //隙間があるとき評価値の集まりを返す
    auto gap_evaluation=[&frame , &polygon](int frame_point_index , int polygon_point_index){
        //NEPの要素数をマイナス1するだけのらむだ
        auto minus_one = [](procon::NeoExpandedPolygon nep , int index){
            index =index - 1;
            if(index == -1) index = nep.getSize();
            return index;
        };
        //
        auto length = [&frame , &polygon](int frame_index , int polygon_index){
            double frame_length = frame.getSideLength().at(frame_index);
            double polygon_length = polygon.getSideLength().at(polygon_index);
            if(frame_length == polygon_length) return 1;
            else return 0;
        };

        int frame_side_index1 = minus_one(frame , frame_point_index);
        int frame_side_index2 = frame_point_index;
        int polygon_side_index1 = minus_one(polygon , polygon_point_index);
        int polygon_side_index2 = polygon_point_index;

        //それぞれ評価値 , frame_side_index , polygon_side_index , frame_point_index , polygon_point_index
        typedef std::tuple<int , int , int , int , int> my_tuple;
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
                        length(frame_side_index1 , polygon_side_index2),
                        frame_side_index1,
                        polygon_side_index2,
                        frame_point_index,
                        polygon_point_index
                    )
        );
        vector.push_back(
                    my_tuple(
                        length(frame_side_index2 , polygon_side_index1),
                        frame_side_index2,
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

    int frame_point_index , polygon_point_index;
    std::vector<std::tuple<int , int , int , int , int>> vector;
    for(frame_point_index = 0 ; frame_point_index < frame.getSize() ; frame_point_index++){
        for(polygon_point_index = 0 ; polygon_point_index < polygon.getSize() ; polygon_point_index++){
            if(angle_status(frame_point_index , polygon_point_index) == 1){
                //フレームとポリゴンの角がちょうどあっているとき
            }else if(angle_status(frame_point_index , polygon_point_index) == 0){
                //角に隙間があるとき
                std::vector<std::tuple<int , int  , int , int , int>> v = gap_evaluation(frame_point_index , polygon_point_index);
                vector.insert(vector.end() , v.begin() , v.end());
            }else if(angle_status(frame_point_index , polygon_point_index) == -1){
                //ポリゴンの角がフレームの角より大きくてありえんとき
            }
        }
    }

    return vector;
}
