#include "algorithmwrapper.h"
#include "utilities.h"

#include <math.h>

#define PI 3.141592

procon::Field AlgorithmWrapper::run(procon::Field field)
{
    return field;
}

int AlgorithmWrapper::searchSameLength(procon::ExpandedPolygon polygon1, procon::ExpandedPolygon polygon2, std::vector<std::array<Fit,2>> &result)
{
    /*許容誤差指定*/
    constexpr double length_error = 0.05; // 単位CM
    constexpr double angle_error = 0.017; //単位rad 0.017rad=1°
    double comped_first = 0;
    double comping_first = 0;
    int Eva = 0;
    int maxEva = 0;
    std::array<Fit,2> fits;
    for (int i = 0; i < polygon1.getSize(); ++i) { //角度のみ探索
        comped_first = polygon1.getSideAngle()[i];
        for (int j = 0; j < polygon2.getSize(); ++j) {
            comping_first = polygon2.getSideAngle()[j];
            if (procon::nearlyEqual(comped_first,comping_first,angle_error)) {
                Eva++;

                int start_polygon1 = i;
                int start_polygon2 = j;
                AlgorithmWrapper::findEnd(polygon2, polygon1, start_polygon2, start_polygon1,length_error, angle_error, Eva);
                fits.at(0).start_dot_or_line = Line;
                fits.at(0).start_id=start_polygon1;
                fits.at(1).start_dot_or_line = Line;
                fits.at(1).start_id=start_polygon2;


                start_polygon1 = i;
                start_polygon2 = j;
                AlgorithmWrapper::findEnd(polygon1, polygon2, start_polygon1, start_polygon2,length_error, angle_error, Eva);
                fits.at(0).end_dot_or_line = Line;
                fits.at(0).end_id=start_polygon1;
                fits.at(1).end_dot_or_line = Line;//Now, It has no meaning (For aborting more bug.)
                fits.at(1).end_id=start_polygon2;
                if (Eva < 2){
                    Eva = 0;
                } else {
                    result.push_back(fits);
                }

                if (Eva > maxEva){
                    maxEva = Eva;
                }
                Eva = 0;
            }
        }
    }
    return maxEva;
}

void AlgorithmWrapper::findEnd(procon::ExpandedPolygon polygon1, procon::ExpandedPolygon polygon2,int &comp1,int &comp2, double length_error, double angle_error, int &Eva)
{
    double comped;
    double comping;
    for (int r=0; r<polygon1.getSize(); r++){
        comp1--;
        if (comp1 < 0) {
            comp1=polygon1.getSize()-1;
        }
        comped=polygon1.getSideLength()[comp1];
        comping=polygon2.getSideLength()[comp2];
        if (comped - (length_error*2) < comping && comping < comped + (length_error*2)){
            Eva++;
        } else {
            if (comp1 == polygon1.getSize()-1){
                comp1=-1;
            }
            comp1++;
            return;

        }

        comp2++;
        if (comp2 > polygon2.getSize()-1){
            comp2=0;
        }
        comped=polygon1.getSideAngle()[comp1];
        comping=polygon2.getSideAngle()[comp2];
        if ((M_PI * 2) - angle_error * 2 < (comping + comped) && (comping + comped) < (M_PI * 2) + angle_error * 2){
            Eva++;
        } else {
            if (comp2 == 0){
                comp2=polygon2.getSize();
            }
            comp2--;
            return;
        }
    }
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
}
