#include "evaluation.h"
#include "neoexpandedpolygon.h"
#include "Utils/polygonconnector.h"

Evaluation::Evaluation()
{

}

std::vector<std::pair<double , Connect>> Evaluation::evaluation(procon::NeoExpandedPolygon const& frame , procon::NeoExpandedPolygon const& polygon)
{
    //要素数の計算
    auto calculation_nep = [](const procon::NeoExpandedPolygon &nep , int index , int cal){
        index = index + cal;
        index = index % nep.getSize();
        if(index < 0){
            index = nep.getSize() + index;
        }
        return index;
    };

    //要素があるかないかの判定
    auto is_there_element = [](const std::vector<std::pair<int , int>> &vector , int frame_point_index , int polygon_point_index){
        for(std::pair<int , int> i :vector){
            if((i.first == frame_point_index) && (i.second == polygon_point_index)) return true;
        }
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

    auto snuggle_up_counter = [&frame , &polygon , &calculation_nep , &angle_status , &length_status]
            (int frame_point_index , int polygon_point_index){
        //辺に沿って角の大きさや辺の長さが合わなくなるまでカウント
        int length_agreement , angle_agreement;
        int trigger_count = 0;
        do{
            trigger_count++;
            int calculated_frame_index = calculation_nep(frame , frame_point_index , trigger_count);
            int calculated_polygon_index = calculation_nep(polygon , polygon_point_index , trigger_count);

            length_agreement = length_status(calculated_frame_index , calculated_polygon_index);
            angle_agreement = angle_status(calculated_frame_index , calculated_polygon_index);
        }while(angle_agreement && length_agreement);

        //行き着いた先がありえない角だったら排除
        if(angle_agreement == -1) return 0;
        //行き着いた先の角がぴったりだったら気持ちだけ(0.5)足してあげる
        if(angle_agreement == 1) trigger_count + 0.5;
        return trigger_count;
    };

    std::vector<std::pair<double , Connect>> vector;
    //すでに通ったことのあるところのchecker
    std::vector<std::pair<int , int>> passed_checker;
    for(int frame_point_index = 0 ; frame_point_index < frame.getSize() ; frame_point_index++){
        for(int polygon_point_index = 0 ; polygon_point_index < polygon.getSize() ; polygon_point_index++){

            int angle_agreement = angle_status(frame_point_index , polygon_point_index);
            if (angle_agreement == -1) continue;
            bool length_agreement = length_status(frame_point_index , polygon_point_index);
            bool passed = is_there_element(passed_checker , frame_point_index , polygon_point_index);

            Connect connect1;
            connect1.frame_side_index = calculation_nep(frame , frame_point_index , -1);
            connect1.polygon_side_index = calculation_nep(polygon , polygon_point_index , -1);
            connect1.frame_point_index = frame_point_index;
            connect1.polygon_point_index = polygon_point_index;

            Connect connect2;
            connect2.frame_side_index = frame_point_index;
            connect2.polygon_side_index = polygon_point_index;
            connect2.frame_point_index = frame_point_index;
            connect2.polygon_point_index = polygon_point_index;

            int snuggle_up = 0;
            //辺が同じだったとき
            if(length_agreement){
                snuggle_up = snuggle_up_counter(frame_point_index , polygon_point_index);
                for(int i = 1; i <= snuggle_up ; i ++) passed_checker.push_back(std::pair<int , int>(frame_point_index + i , polygon_point_index + i));
            }

            if((!passed) && (angle_agreement == 1)){
                //角が同じだったとき
                double evaluation = 1;
                if(snuggle_up > 0) evaluation = std::pow(snuggle_up , 2);
                vector.push_back((std::pair<double , Connect>(evaluation , connect1)));
            }else if(angle_agreement == 0){
                //角に隙間があるとき
                if(!passed) vector.push_back((std::pair<double , Connect>(0 , connect1)));
                double evaluation = 0;
                if(snuggle_up > 0) evaluation = std::pow(snuggle_up , 2);
                vector.push_back((std::pair<double , Connect>(evaluation , connect2)));
            }
        }
    }

    return vector;
}
