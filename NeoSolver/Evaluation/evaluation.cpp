#include "evaluation.h"
#include "neoexpandedpolygon.h"

Evaluation::Evaluation()
{

}


std::vector<std::tuple<int , int , int >> Evaluation::evaluation(procon::NeoExpandedPolygon const& frame , procon::NeoExpandedPolygon const& polygon)
{
    //それぞれのNEPの頂点を引数として渡し、評価値を返す
    auto main_evaluation=[frame , polygon](int frame_index , int polygon_index){
        //角について評価
        auto about_angle = [frame , polygon , frame_index , polygon_index](){
            double frame_angle = frame.getSideAngle().at(frame_index);
            double polygon_angle = polygon.getSideAngle().at(polygon_index);
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
        //辺について評価
        auto about_length = [frame , polygon , frame_index , polygon_index](){
            int a = frame_index - 1;
            if(a == -1) a = frame.getSize() - 1;
            double frame_length1 = frame.getSideLength().at(a);
            double field_length2 = frame.getSideLength().at(frame_index);

            a = polygon_index - 1;
            if(a == -1) a = polygon.getSize() - 1;
            double polygon_length1 = polygon.getSideLength().at(a);
            double polygon_length2 = polygon.getSideLength().at(polygon_index);

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

        //上の結果を集計する
        int point = 0;
        point = point + about_angle();
        if(point < 0) return point;
        point = point + about_length();
        return point;
    };

    //それぞれ評価値、フレームのインデックス、ポリゴンのインデックス
    int evaluation , frame_index , polygon_index;
    std::vector<std::tuple<int , int , int>> vector;
    for(frame_index = 0 ; frame_index < frame.getSize() ; frame_index++){
        for(polygon_index = 0 ; polygon_index < polygon.getSize() ; polygon_index++){
            evaluation = main_evaluation(frame_index , polygon_index);
            vector.push_back(std::tuple<int , int , int>(evaluation , frame_index , polygon_index));
        }
    }

    return vector;
}
