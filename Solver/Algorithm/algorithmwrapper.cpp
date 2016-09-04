#include "algorithmwrapper.h"

#include <math.h>

#define PI 3.141592

procon::Field AlgorithmWrapper::run(procon::Field field)
{
    return field;
}

procon::ExpandedPolygon AlgorithmWrapper::newPolygonDate(procon::ExpandedPolygon polygon)
{
    int size = polygon.getSize();
    polygon_t newPolygon;
    for (int i=0; i<size; i++) {
        double x = polygon.getPolygon().outer()[i].x();
        double y = polygon.getPolygon().outer()[i].y();
        newPolygon.outer().push_back(point_t(x,y));
    }
    procon::ExpandedPolygon change_Polygon;
    change_Polygon.setPolygon(newPolygon);
    return change_Polygon;
}

procon::ExpandedPolygon AlgorithmWrapper::joinPolygon(procon::ExpandedPolygon Polygon1,int Polygon1_start_pos,int Polygon1_end_pos, procon::ExpandedPolygon Polygon2,int Polygon2_start_pos,int Polygon2_end_pos)
{
<<<<<<< HEAD
    int size1 = Polygon1.getSize();
    int size2 = Polygon2.getSize();
    Polygon1 = newPolygonDate(Polygon1);
    Polygon2 = newPolygonDate(Polygon2);
=======
    // 各ポリゴンのstartからend方面の次の角までのベクトルから、ポリゴンの傾きを調べる。
    int size1 = Polygon1.getSize() + 1;
    int size2 = Polygon2.getSize() + 1;
>>>>>>> c65139a0e53200cd947eccfb40a318f1cedfeb6c
    double x1 = Polygon1.getPolygon().outer()[(Polygon1_start_pos + 1) % size1].x() - Polygon1.getPolygon().outer()[Polygon1_start_pos].x();
    double y1 = Polygon1.getPolygon().outer()[(Polygon1_start_pos + 1) % size1].y() - Polygon1.getPolygon().outer()[Polygon1_start_pos].y();
    double x2 = Polygon2.getPolygon().outer()[(Polygon2_start_pos + size2 - 1) % size2].x() - Polygon2.getPolygon().outer()[Polygon2_start_pos].x();
    double y2 = Polygon2.getPolygon().outer()[(Polygon2_start_pos + size2 - 1) % size2].y() - Polygon2.getPolygon().outer()[Polygon2_start_pos].y();
    double degree2 = atan2(y2, x2);
    double degree1 = atan2(y1, x1);
    double degree = (degree1 - degree2) * 180 / PI;

    // Polygon2を回転させる。このとき誤差が生じる。
    double rad = degree * PI / 180;
    polygon_t turn_polygon;
    for (int i=0; i<size2; i++)
    {
        double x = Polygon2.getPolygon().outer()[i].x() - Polygon2.getPolygon().outer()[0].x();
        double y = Polygon2.getPolygon().outer()[i].y() - Polygon2.getPolygon().outer()[0].y();
        double move_x = (x * cos(rad)) - (y * sin(rad));
        double move_y = (x * sin(rad)) + (y * cos(rad));
        double turn_x = move_x + Polygon2.getPolygon().outer()[0].x();
        double turn_y = move_y + Polygon2.getPolygon().outer()[0].y();
        turn_polygon.outer().push_back(point_t(turn_x, turn_y));
    }
    Polygon2.setPolygon(turn_polygon);

    // 結合後完全に一致する点からポリゴンのx,y移動を調べ、Polygon2を平行移動
    int Join_point1 = (Polygon1_start_pos + 1) % size1;
    int Join_point2 = (Polygon2_start_pos + size2 - 1) % size2;
    double move_x = Polygon1.getPolygon().outer()[Join_point1].x() - Polygon2.getPolygon().outer()[Join_point2].x();
    double move_y = Polygon1.getPolygon().outer()[Join_point1].y() - Polygon2.getPolygon().outer()[Join_point2].y();

    Polygon2.translatePolygon(move_x, move_y);

    polygon_t new_pieces;
    int count = Polygon1_end_pos + 1;
    int Type = 1;

    // ポリゴンの結合端の辺の長さ
    double x1_st = Polygon1.getPolygon().outer()[(Polygon1_start_pos + 1) % size1].x() - Polygon1.getPolygon().outer()[Polygon1_start_pos].x();
    double y1_st = Polygon1.getPolygon().outer()[(Polygon1_start_pos + 1) % size1].y() - Polygon1.getPolygon().outer()[Polygon1_start_pos].y();
    double x2_st = Polygon2.getPolygon().outer()[(Polygon2_start_pos + size2 - 1) % size1].x() - Polygon2.getPolygon().outer()[Polygon2_start_pos].x();
    double y2_st = Polygon2.getPolygon().outer()[(Polygon2_start_pos + size2 - 1) % size1].y() - Polygon2.getPolygon().outer()[Polygon2_start_pos].y();
    double x1_end = Polygon1.getPolygon().outer()[(Polygon1_end_pos + size1 - 1) % size1].x() - Polygon1.getPolygon().outer()[Polygon1_end_pos].x();
    double y1_end = Polygon1.getPolygon().outer()[(Polygon1_end_pos + size1 - 1) % size1].y() - Polygon1.getPolygon().outer()[Polygon1_end_pos].y();
    double x2_end = Polygon2.getPolygon().outer()[(Polygon2_end_pos + 1) % size1].x() - Polygon2.getPolygon().outer()[Polygon2_end_pos].x();
    double y2_end = Polygon2.getPolygon().outer()[(Polygon2_end_pos + 1) % size1].y() - Polygon2.getPolygon().outer()[Polygon2_end_pos].y();

<<<<<<< HEAD
    double x,y;
    do{
        if (Type == 1){
=======
    // 新しいポリゴンに結合後の外周の角を入れる。
    // もし、結合端の辺の長さが等しくならない時はPolygon1,Polygon2ともに端の角を入力。
    // ここで回転の誤差により角が一致しない場合がある。
    do
    {
        double x,y;
        if (Type == 1)
        {
>>>>>>> c65139a0e53200cd947eccfb40a318f1cedfeb6c
            x = Polygon1.getPolygon().outer()[count%size1].x();
            y = Polygon1.getPolygon().outer()[count%size1].y();
            if (count % size1 == Polygon1_start_pos){
                Type = 2;
                if ((x1_st == x2_st) && (y1_st == y2_st)){
                    count = Polygon2_start_pos;
                } else {
                    count = Polygon2_start_pos - 1;
                }
            }
        } else if (Type == 2) {
            x = Polygon2.getPolygon().outer()[count%size2].x();
            y = Polygon2.getPolygon().outer()[count%size2].y();
            if (count % size2 == Polygon2_end_pos) {
                Type = 1;
                if ((x1_end == x2_end) && (y1_end == y2_end)){
                    count = Polygon1_end_pos;
                } else {
                    count = Polygon1_end_pos - 1;
                }
            }
        }
        new_pieces.outer().push_back(point_t(x,y));
        count++;
    } while ((Type != 1) || (count != (Polygon1_end_pos + 1)));

    x = Polygon1.getPolygon().outer()[count].x();
    y = Polygon1.getPolygon().outer()[count].y();
    new_pieces.outer().push_back(point_t(x,y));

    procon::ExpandedPolygon new_polygon;
    new_polygon.setPolygon(new_pieces);
    return new_polygon;
}

AlgorithmWrapper::AlgorithmWrapper()
{
    // サンプル
    polygon_t sample11;
    sample11.outer().push_back(point_t(0,0));
    sample11.outer().push_back(point_t(0,2));
    sample11.outer().push_back(point_t(1,2));
    sample11.outer().push_back(point_t(1,1));
    sample11.outer().push_back(point_t(2,1));
    sample11.outer().push_back(point_t(2,0));
    sample11.outer().push_back(point_t(0,0));

    polygon_t sample12;
    sample12.outer().push_back(point_t(0,0));
    sample12.outer().push_back(point_t(0,1));
    sample12.outer().push_back(point_t(2,2));
    sample12.outer().push_back(point_t(2,0));
    sample12.outer().push_back(point_t(0,0));

    procon::ExpandedPolygon polygon1(0);
    procon::ExpandedPolygon polygon2(1);
    polygon1.setPolygon(sample11);
    polygon2.setPolygon(sample12);

    procon::ExpandedPolygon Joined = joinPolygon(polygon1,2,4,polygon2,1,3);
}
