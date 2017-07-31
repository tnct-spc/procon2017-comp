#include "algorithmwrapper.h"

AlgorithmWrapper::AlgorithmWrapper()
{

}


AlgorithmWrapper::~AlgorithmWrapper()
{

}

void AlgorithmWrapper::init(){

}

void AlgorithmWrapper::run(procon::NeoField field)
{
    std::cout << "algorithmwarpper called" << std::endl;
    return;
}

void AlgorithmWrapper::submitAnswer(procon::NeoField field)
{
    std::cout << "algorithm warpper throwanswr called" << std::endl;
    emit throwAnswer(field);
    // Wait 1msec
    QEventLoop loop;
    QTimer::singleShot(1, &loop, SLOT(quit()));
    loop.exec();
}

int evaluation(procon::NeoExpandedPolygon field , procon::NeoExpandedPolygon polygon)
{
    //引数は頂点の番号　0はじまり
    auto main_evaluation=[field,polygon](int field_point,int polygon_point){
        auto about_angle = [field,polygon,field_point,polygon_point](){
            double field_angle = field.getSideAngle().at(field_point);
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
        auto about_length = [field,polygon,field_point,polygon_point](){
            int a = field_point - 1;
            if(a == -1) a = field.getSize() - 1;
            double field_length1 = field.getSideLength().at(a);
            double field_length2 = field.getSideLength().at(field_point);

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

        int point=0;
        point = point + about_angle();
        //いまは角の大きさ重視
        if(point < 0) return point;
        point = point + about_length();
        return point;
    };

    int max=0;
    int field_index = -1 , polygon_index = -1;
    for(int i = 0 ; i < field.getSize() ; i++){
       for(int j = 0 ; j < polygon.getSize() ; j++){
           int a = main_evaluation(i,j);
           if(a > max){
               max = a;
               field_index = i;
               polygon_index = j;
           }
       }
    }

}
