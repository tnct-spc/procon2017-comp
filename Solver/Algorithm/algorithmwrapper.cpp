#include "algorithmwrapper.h"

#include <math.h>

#define PI 3.141592

procon::Field AlgorithmWrapper::run(procon::Field field)
{
    return field;
}

// 終点を抜く。
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
    Polygon1.updatePolygon();
    Polygon2.updatePolygon();
    int size1 = Polygon1.getSize();
    int size2 = Polygon2.getSize();
    Polygon1 = newPolygonDate(Polygon1);
    Polygon2 = newPolygonDate(Polygon2);

    const std::vector<point_t> ring1 = Polygon1.getPolygon().outer();
    const std::vector<point_t> ring2 = Polygon2.getPolygon().outer();

  //結合面内の角が合計３６０度にならなかったらポリゴン１を返す
    int inner_point1 = Polygon1_start_pos + 1;
    int inner_point2 = Polygon2_start_pos - 1;
    Polygon1.getSideAngle();
    Polygon2.getSideAngle();
    while (((inner_point1 % size1) != Polygon1_end_pos) && ((inner_point2 + size2) % size2 != Polygon2_end_pos)) {
        double angle1 = Polygon1.getSideAngle()[inner_point1 % size1];
        double angle2 = Polygon2.getSideAngle()[(inner_point2 + size2) % size2];
        if (fabs(2 * PI - angle1 - angle2) > (PI / 180)) {
            std::cerr << "polygon error" << std::endl;
            return Polygon1;
        }
        inner_point1++;
        inner_point2--;
    }

    const double x1 = ring1[(Polygon1_start_pos + 1) % size1].x() - ring1[Polygon1_start_pos].x();
    const double y1 = ring1[(Polygon1_start_pos + 1) % size1].y() - ring1[Polygon1_start_pos].y();
    const double x2 = ring2[(Polygon2_start_pos + size2 - 1) % size2].x() - ring2[Polygon2_start_pos].x();
    const double y2 = ring2[(Polygon2_start_pos + size2 - 1) % size2].y() - ring2[Polygon2_start_pos].y();
    const double degree2 = atan2(y2, x2);
    const double degree1 = atan2(y1, x1);
    const double degree = (degree1 - degree2) * 180 / PI;

    // Polygon2を回転させる。このとき誤差が生じる。
    const double rad = degree * PI / 180;
    polygon_t turn_polygon;
    for (int i=0; i<size2; i++)
    {
        const double x = ring2[i].x() - ring2[0].x();
        const double y = ring2[i].y() - ring2[0].y();
        const double move_x = (x * cos(rad)) - (y * sin(rad));
        const double move_y = (x * sin(rad)) + (y * cos(rad));
        const double turn_x = move_x + ring2[0].x();
        const double turn_y = move_y + ring2[0].y();
        turn_polygon.outer().push_back(point_t(turn_x, turn_y));
    }
    Polygon2.setPolygon(turn_polygon);

    // 結合後完全に一致する点からポリゴンのx,y移動を調べ、Polygon2を平行移動
    const int Join_point1 = (Polygon1_start_pos + 1) % size1;
    const int Join_point2 = (Polygon2_start_pos + size2 - 1) % size2;
    const double move_x = ring1[Join_point1].x() - ring2[Join_point2].x();
    const double move_y = ring1[Join_point1].y() - ring2[Join_point2].y();

    Polygon2.translatePolygon(move_x, move_y);

    polygon_t new_pieces;
    int count = Polygon1_end_pos + 1;
    int Type = 1;

    // ポリゴンの結合端の辺の長さ
    const double st1 = Polygon1.getSideLength()[Polygon1_start_pos];
    const double st2 = Polygon2.getSideLength()[(Polygon2_start_pos + size2 - 1) % size2];
    const double end1 = Polygon1.getSideLength()[(Polygon1_end_pos + size1 - 1) % size1];
    const double end2 = Polygon2.getSideLength()[Polygon2_end_pos];

    // 新しいポリゴンに結合後の外周の角を入れる。
    // もし、結合端の辺の長さが等しくならない時はPolygon1,Polygon2ともに端の角を入力。
    // ここで回転の誤差により角が一致しない場合がある。
    double x,y;
    do{
        if (Type == 1) {
            x = ring1[count%size1].x();
            y = ring1[count%size1].y();
            if (count % size1 == Polygon1_start_pos){
                Type = 2;
                if (fabs(st1 - st2) <= 0.1) {
                    count = Polygon2_start_pos;
                } else {
                    count = Polygon2_start_pos - 1;
                }
            }
        } else if (Type == 2) {
            x = ring2[count%size2].x();
            y = ring2[count%size2].y();
            if (count % size2 == Polygon2_end_pos) {
                Type = 1;
                if (fabs(end1 - end2) <= 0.1) {
                    count = Polygon1_end_pos;
                } else {
                    count = Polygon1_end_pos - 1;
                }
            }
        }
        new_pieces.outer().push_back(point_t(x,y));
        count++;
    } while ((Type != 1) || (count != (Polygon1_end_pos + 1)));

    // 終点を足してから返す。
    x = ring1[count].x();
    y = ring1[count].y();
    new_pieces.outer().push_back(point_t(x,y));

    procon::ExpandedPolygon new_polygon;
    new_polygon.setPolygon(new_pieces);
    return new_polygon;
}

AlgorithmWrapper::AlgorithmWrapper()
{
    // サンプル
    polygon_t sample11;
    sample11.outer().push_back(point_t(0.00000001,0.0000002));
    sample11.outer().push_back(point_t(0.00000008,6.0000007));
    sample11.outer().push_back(point_t(2.00000004,2.0000003));
    sample11.outer().push_back(point_t(4.00000009,2.0000006));
    sample11.outer().push_back(point_t(6.00000005,0.0000004));
    sample11.outer().push_back(point_t(0.00000001,0.0000002));

    polygon_t sample12;
    sample12.outer().push_back(point_t(0.00000004,0.00000002));
    sample12.outer().push_back(point_t(-1.00000003,2.00000009));
    sample12.outer().push_back(point_t(4.00000009,2.00000002));
    sample12.outer().push_back(point_t(4.00000006,-2.00000005));
    sample12.outer().push_back(point_t(3.00000005,0.00000004));
    sample12.outer().push_back(point_t(0.00000004,0.00000002));

    procon::ExpandedPolygon polygon1(0);
    procon::ExpandedPolygon polygon2(1);
    polygon1.setPolygon(sample11);
    polygon2.setPolygon(sample12);

    procon::ExpandedPolygon Joined = joinPolygon(polygon1,1,4,polygon2,1,3);
    std::cout << "end" << std::endl;
}
