#include "evaluation.h"
#include "neoexpandedpolygon.h"

Evaluation::Evaluation()
{

}


std::vector<std::tuple<int , int , int , int>> Evaluation::evaluation(std::vector<procon::NeoExpandedPolygon> field , procon::NeoExpandedPolygon polygon)
{
    //引数は頂点の番号　0はじまり
    auto main_evaluation=[](procon::NeoExpandedPolygon one_field,int field_point,procon::NeoExpandedPolygon polygon,int polygon_point){
        auto about_angle = [one_field,polygon,field_point,polygon_point](){
            double field_angle = one_field.getSideAngle().at(field_point);
            double polygon_angle = polygon.getSideAngle().at(polygon_point);
            if(field_angle == polygon_angle){
                //フレームとポリゴンの角がちょうどあっているとき
                return 1;
            }else if(field_angle > polygon_angle){
                //角に隙間があるとき
                return -1;
            }else if(field_angle < polygon_angle){
                //ポリゴンの角がフレームの角より大きくてありえんとき
                return -2;
            }
        };
        auto about_length = [one_field,polygon,field_point,polygon_point](){
            int a = field_point - 1;
            if(a == -1) a = one_field.getSize() - 1;
            double field_length1 = one_field.getSideLength().at(a);
            double field_length2 = one_field.getSideLength().at(field_point);

            a = polygon_point - 1;
            if(a == -1) a = polygon.getSize() - 1;
            double polygon_length1 = polygon.getSideLength().at(a);
            double polygon_length2 = polygon.getSideLength().at(polygon_point);

            bool b = field_length1 == polygon_length1;
            bool c = field_length2 == polygon_length2;
            bool d = field_length1 == polygon_length2;
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

    int field_vector_index = -1,field_index = -1 , polygon_index = -1;

    std::vector<std::tuple<int , int , int , int>> vector;
    for(int i = 0 ; i < field.size() ; i++){
       for(int j = 0 ; j < polygon.getSize() ; j++){
           for(int k = 0 ; k < field.at(i).getPolygon().outer().size() - 1 ; k++){
               int a = main_evaluation(field.at(i),k,polygon,j);
               if(a > 0){
                   field_vector_index = i;
                   field_index = k;
                   polygon_index = j;

                   vector.push_back(std::tuple<int , int , int , int>(field_vector_index,field_index,polygon_index,a));
               }
           }
       }
    }
    return vector;
}
