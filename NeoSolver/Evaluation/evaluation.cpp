#include "evaluation.h"
#include "neoexpandedpolygon.h"

Evaluation::Evaluation()
{

}


std::vector<std::tuple<int , int , int >> Evaluation::evaluation(procon::NeoExpandedPolygon const& frame , procon::NeoExpandedPolygon const& polygon)
{
    //引数は頂点の番号　0はじまり
    auto main_evaluation=[](procon::NeoExpandedPolygon one_frame,int frame_point,procon::NeoExpandedPolygon polygon,int polygon_point){
        auto about_angle = [one_frame,polygon,frame_point,polygon_point](){
            double frame_angle = one_frame.getSideAngle().at(frame_point);
            double polygon_angle = polygon.getSideAngle().at(polygon_point);
            if(frame_angle == polygon_angle){
                //フレームとポリゴンの角がちょうどあっているとき
                return 1;
            }else if(frame_angle > polygon_angle){
                //角に隙間があるとき
                return -1;
            }else if(frame_angle < polygon_angle){
                //ポリゴンの角がフレームの角より大きくてありえんとき
                return -2;
            }
        };
        auto about_length = [one_frame,polygon,frame_point,polygon_point](){
            int a = frame_point - 1;
            if(a == -1) a = one_frame.getSize() - 1;
            double frame_length1 = one_frame.getSideLength().at(a);
            double field_length2 = one_frame.getSideLength().at(frame_point);

            a = polygon_point - 1;
            if(a == -1) a = polygon.getSize() - 1;
            double polygon_length1 = polygon.getSideLength().at(a);
            double polygon_length2 = polygon.getSideLength().at(polygon_point);

            bool b = frame_length1 == polygon_length1;
            bool c = field_length2 == polygon_length2;
            bool d = frame_length1 == polygon_length2;
            bool e = field_length2 == polygon_length1;
            if((b && c) || (d && e)){
                //2辺ちょうどあっている
                return 2;
            }else if(b || c || d || e){
                //1辺どちらかがあっている
                return 1;
            }else{
                return 0;
            }
        };

        int point = 0;
        point = point + about_angle();
        //いまは角の大きさ重視
        if(point < 0) return point;
        point = point + about_length();
        return point;
    };

    int evaluation = -1 , frame_index = -1 , polygon_index = -1;

    std::vector<std::tuple<int , int , int >> vector;
    for(int k = 0 ; k < frame.getSize() ; k++){
        for(int j = 0 ; j < polygon.getSize() ; j++){
            evaluation = main_evaluation(frame,k,polygon,j);
            frame_index = k;
            polygon_index = j;

            vector.push_back(std::tuple<int , int , int>(evaluation , frame_index , polygon_index));
        }
    }

    return vector;
}
