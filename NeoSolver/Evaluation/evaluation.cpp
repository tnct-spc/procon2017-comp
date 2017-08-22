#include "evaluation.h"
#include "neoexpandedpolygon.h"
#include "Utils/polygonconnector.h"

Evaluation::Evaluation()
{

}

//要素数の計算
int Evaluation::calculation_nep(const procon::NeoExpandedPolygon &nep , int index , int cal){
    index = index + cal;
    index = index % nep.getSize();
    if(index < 0){
        index = nep.getSize() + index;
    }
    return index;
}

std::vector<std::pair<double , Connect>> Evaluation::evaluation(
        procon::NeoExpandedPolygon const& frame , procon::NeoExpandedPolygon const& polygon , double angle_weight , double side_weight)
{
    //一番貴重なときの評価値の大きさ
    const double max_precious = 0.5;

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

    //どれだけ辺に寄り添ってきたか
    auto snuggle_up_counter = [&frame , &polygon , &angle_status , &length_status]
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
            //無限ループ回避
            if(trigger_count == polygon.getSize()) break;
        }while(angle_agreement && length_agreement);

        double snuglle_up;
        //行き着いた先がありえない角だったら排除
        if(angle_agreement == -1) return 0.0;
        //行き着いた先の角がぴったりだったら気持ちだけ(0.5)足してあげる
        snuglle_up = trigger_count;
        if(angle_agreement == 1) snuglle_up = trigger_count + 0.5;
        return snuglle_up;
    };

    //同じ角度を持つものは何個あるか調べる
    auto count_same_angle = [](const procon::NeoExpandedPolygon &polygon){
        std::vector<std::pair<double , int>> map;
        std::vector<double> vector = polygon.getSideAngle();
        sort(vector.begin() , vector.end(), [](double angle1 , double angle2){ return angle1 < angle2; });

        double before_angle = -1;
        int count;
        for(double i : vector){
            if(i != before_angle){
                count = 1;
                map.push_back(std::pair<double , int>(i , count));
            }else{
                auto itr = map.end() - 1;
                *itr = std::pair<double , int>(i , count);
            }
            count++;
            before_angle = i;
        }
        //vectorをmapに変換
        std::map<double , int> map2;
        for(std::pair<double , int> i : map){
            map2.insert(i);
        }
        return map2;
    };

    //同じ角度の角は何個あるか示すmap
    std::map<double , int> frame_same_angle = count_same_angle(frame);
    std::map<double , int> polygon_same_angle = count_same_angle(polygon);
    //すでに通ったことのあるところのchecker
    std::vector<std::pair<int , int>> passed_checker;

    //どれだけ辺に寄り添ってきたか(最初によばれるほう)
    auto snuggle_processing = [&](int frame_point_index , int polygon_point_index){
        double snuggle_up = snuggle_up_counter(frame_point_index , polygon_point_index);
        //通ってきた道のしるしづけ
        for(int i = 1; i <= snuggle_up ; i ++){
            int calculated_frame_index = calculation_nep(frame , frame_point_index , i);
            int calculated_polygon_index = calculation_nep(polygon , polygon_point_index , i);
            passed_checker.push_back(std::pair<int , int>(calculated_frame_index , calculated_polygon_index));
        }
        //通ってきた道がいかに貴重だったかを確認する
        double precious_degree = 0;
        for(int i = 1; i <= snuggle_up ; i ++){
            int calculated_frame_index = calculation_nep(frame , frame_point_index , i);
            int calculated_polygon_index = calculation_nep(polygon , polygon_point_index , i);

            double precious_frame_degree = frame_same_angle[frame.getSideAngle().at(calculated_frame_index)];
            double precious_polygon_degree = polygon_same_angle[polygon.getSideAngle().at(calculated_polygon_index)];
            precious_degree += max_precious / (precious_frame_degree * precious_polygon_degree);
        }
        snuggle_up += precious_degree;
        return snuggle_up;
    };

    //返すときに使うvector
    std::vector<std::pair<double , Connect>> vector;

    //二重ループ開始
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

            double snuggle_up = 0;
            //辺が同じだったとき
            if(length_agreement) snuggle_up = snuggle_processing(frame_point_index , polygon_point_index);

            if((!passed) && (angle_agreement == 1)){
                //角が同じだったとき
                double evaluation = angle_weight;

                //角の貴重度
                int precious_frame_degree = frame_same_angle[frame.getSideAngle().at(frame_point_index)];
                int precious_polygon_degree = polygon_same_angle[polygon.getSideAngle().at(polygon_point_index)];
                double precious_degree = max_precious / (precious_frame_degree * precious_polygon_degree);

                if(snuggle_up > 0) evaluation = std::pow(snuggle_up , 2);
                vector.push_back((std::pair<double , Connect>(evaluation + precious_degree, connect1)));
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
