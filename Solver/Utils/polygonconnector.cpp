#include "polygonconnector.h"

PolygonConnector::PolygonConnector()
{

}

// 終点を抜く。
procon::ExpandedPolygon PolygonConnector::newPolygonDate(procon::ExpandedPolygon polygon)
{
    int size = polygon.getSize();
    polygon_t newPolygon;
    for (int i=0; i<size; i++) {
        double x = polygon.getPolygon().outer()[i].x();
        double y = polygon.getPolygon().outer()[i].y();
        newPolygon.outer().push_back(point_t(x,y));
    }
    polygon.setPolygon(newPolygon);
    return polygon;
}

procon::ExpandedPolygon PolygonConnector::joinPolygon(procon::ExpandedPolygon Polygon1, procon::ExpandedPolygon Polygon2, std::array<Fit,2> join_data)
{
    Fit fit1 = join_data.at(0);
    Fit fit2 = join_data.at(1);
    Polygon1.updatePolygon();
    Polygon2.updatePolygon();
    int size1 = Polygon1.getSize();
    int size2 = Polygon2.getSize();
    Polygon1 = newPolygonDate(Polygon1);
    Polygon2 = newPolygonDate(Polygon2);

    //結合後完全に一致する、始点及び終点
    const int complete_matching_start_pos_1 = fit1.start_dot_or_line == Fit::Dot ? fit1.start_id : fit1.start_id              ;
    const int complete_matching_end_pos_1   = fit1.end_dot_or_line   == Fit::Dot ? fit1.end_id   : (fit1.end_id   + 1) % size1;
    const int complete_matching_start_pos_2 = fit2.start_dot_or_line == Fit::Dot ? fit2.start_id : (fit2.start_id + 1) % size2;
    const int complete_matching_end_pos_2   = fit2.start_dot_or_line == Fit::Dot ? fit2.end_id   : fit2.end_id                ;

    // Polygon2を回転させる。このとき誤差が生じる。
    std::vector<point_t> ring1 = Polygon1.getPolygon().outer();
    std::vector<point_t> ring2 = Polygon2.getPolygon().outer();
    const double x1 = ring1[complete_matching_start_pos_1].x() - ring1[(complete_matching_start_pos_1 + 1) % size1].x();
    const double y1 = ring1[complete_matching_start_pos_1].y() - ring1[(complete_matching_start_pos_1 + 1) % size1].y();
    const double x2 = ring2[complete_matching_start_pos_2].x() - ring2[((complete_matching_start_pos_2 - 1) % size2 + size2) % size2].x();
    const double y2 = ring2[complete_matching_start_pos_2].y() - ring2[((complete_matching_start_pos_2 - 1) % size2 + size2) % size2].y();
    const double degree2 = atan2(y2, x2);
    const double degree1 = atan2(y1, x1);
    const double degree = (degree1 - degree2) * 180 / M_PI;

    const double rad = degree * M_PI / 180;
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

    ring1 = Polygon1.getPolygon().outer();
    ring2 = Polygon2.getPolygon().outer();



    // 結合後完全に一致する点からポリゴンのx,y移動を調べ、Polygon2を平行移動
    const int Join_point1 = complete_matching_start_pos_1;
    const int Join_point2 = complete_matching_start_pos_2;
    const double move_x = ring1[Join_point1].x() - ring2[Join_point2].x();
    const double move_y = ring1[Join_point1].y() - ring2[Join_point2].y();

    Polygon2.translatePolygon(move_x, move_y);

    ring1 = Polygon1.getPolygon().outer();
    ring2 = Polygon2.getPolygon().outer();

    // 新しいポリゴンに結合後の外周の角を入れる。
    // もし、結合端の辺の長さが等しくならない時はPolygon1,Polygon2ともに端の角を入力。
    // ここで回転の誤差により角が一致しない場合がある。
    polygon_t new_pieces;
    int count = complete_matching_end_pos_1 + 1;
    int Type = 1;

    double x,y;
    std::cout<<"roops"<<std::endl;
    do{
        if (Type == 1) {
            x = ring1[count%size1].x();
            y = ring1[count%size1].y();
            std::cout<<"b="<<count % size1<<" pos="<<complete_matching_start_pos_1<<std::endl;
            if (count % size1 == complete_matching_start_pos_1){
                std::cout<<"change!"<<std::endl;
                Type = 2;
                if (fit1.start_dot_or_line == Fit::Dot) { //dot_or_lineはどちらのポリゴンでも同じですね…仕様が変だ
                    count = complete_matching_start_pos_2 + 1;
                } else {
                    count = complete_matching_start_pos_2;
                }
            }else{
                count++;
            }
        }
        if (Type == 2) {
            x = ring2[count%size2].x();
            y = ring2[count%size2].y();
            std::cout<<"c="<<count % size2<<" pos="<<complete_matching_end_pos_2<<(fit2.end_dot_or_line == Fit::Dot ? (complete_matching_end_pos_2 - 1) % size2 : complete_matching_end_pos_2)<<std::endl;
            if (count % size2 == (fit2.end_dot_or_line == Fit::Dot ? (((complete_matching_end_pos_2 - 1) % size2 + size2) % size2) : complete_matching_end_pos_2)) {
                Type = -1;
            }else{
                count++;
            }
        }
        new_pieces.outer().push_back(point_t(x,y));
    } while (Type != -1);
    std::cout<<"roope"<<std::endl;

    //開始点を足してから返す。
    x = ring1[(complete_matching_end_pos_1 + 1) % size1].x();
    y = ring1[(complete_matching_end_pos_1 + 1) % size1].y();
    new_pieces.outer().push_back(point_t(x,y));

    procon::ExpandedPolygon new_polygon(0); //TODO:id
    new_polygon.setPolygon(new_pieces);

    return new_polygon;
}
