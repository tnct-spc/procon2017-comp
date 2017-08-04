#include "polygonconnector.h"
#include "precompile.h"
#include <boost/geometry/algorithms/union.hpp>

PolygonConnector::PolygonConnector()
{

}

std::tuple<std::vector<procon::NeoExpandedPolygon>, procon::NeoExpandedPolygon, bool> PolygonConnector::connect(procon::NeoExpandedPolygon frame, procon::NeoExpandedPolygon piece, Connect connecter)
//フレームにピースをはめる関数
//回転関数により、ピースを原点を中心に回転させたとき、全ての点が整数値の座標を持つようなピースを抽出する。
//ピースとフレームの重なるべき辺において、接すべき点からもう一方の点への角度を複素数平面上で考え、
//抽出されたピースの角度とはめるフレームの角度が合致しているか、調べる。
//角度が合致していた場合、ピースがフレームに重なるように、ピースを移動する。
//ピースがフレームからはみ出してないか調べるため、ピースとフレームを足し合わせたものと、元々のフレームが同じ形かを調べる。
//はみ出していなければ、フレームからピースを引いたものが、フレームにピースをはめた後のフレームとなる。

{
    //ピースとフレームの重なるべき辺において、接すべき点からもう一方の点への角度を複素数平面上で考える
    int frame_index1 = connecter.frame_point_index; //フレームの接すべき点
    int piece_index1 = connecter.polygon_point_index; //ピースの接すべき点

    int frame_index2, piece_index2;
    if(connecter.frame_point_index == connecter.frame_side_index) {
        frame_index2 = (frame_index1 != frame.getSize() - 1) ? frame_index1 + 1 : 0;
        piece_index2 = (piece_index1 != piece.getSize() - 1) ? piece_index1 + 1 : 0;
    } else {
        frame_index2 = (frame_index1 != 0) ? frame_index1 - 1 : frame.getSize() - 1;
        piece_index2 = (piece_index1 != 0) ? piece_index1 - 1 : piece.getSize() - 1;
    }

    std::vector<procon::NeoExpandedPolygon> empty1;
    procon::NeoExpandedPolygon empty2;
    std::tuple<std::vector<procon::NeoExpandedPolygon>, procon::NeoExpandedPolygon, bool> out_empty = std::make_tuple(empty1, empty2, false);
    polygon_i frame_polygon = frame.getPolygon();
    polygon_i piece_polygon = piece.getPolygon();
    std::vector<polygon_i> frame_out_polygons, union_polygons;
    std::vector<point_i> frame_points = frame_polygon.outer();
    point_i frame_point1 = frame_points.at(frame_index1);
    point_i frame_point2 = frame_points.at(frame_index2);
    complex_d frame_complex(static_cast<double> (frame_point2.x() - frame_point1.x()), static_cast<double> (frame_point2.y() - frame_point1.y()));

    std::pair<polygon_i, bool> rotate_out = (this -> rotate(piece_polygon, frame_complex, piece_index1, piece_index2));
    if(!rotate_out.second) return out_empty;
    polygon_i piece_out_polygon = rotate_out.first;
    std::vector<point_i> piece_out_points = piece_out_polygon.outer();
    point_i piece_out_point1 = piece_out_points.at(piece_index1);

    procon::NeoExpandedPolygon piece_out;
    piece_out.resetPolygonForce(piece_out_polygon);
    piece_out.translatePolygon(piece_out_point1.x(), piece_out_point1.y());
    bg::union_(frame_polygon, piece_out_polygon, union_polygons);
    if(union_polygons.size() != 1) return out_empty;
    polygon_i union_polygon = union_polygons.at(0);

    if(bg::equals(frame_polygon, union_polygon)) {
        bg::difference(frame_polygon, piece_out_polygon, frame_out_polygons);
        std::vector<procon::NeoExpandedPolygon> frames_out;

        for(polygon_i frame_out_polygon : frame_out_polygons) {
            procon::NeoExpandedPolygon frame_out;
            frame_out.resetPolygonForce(frame_out_polygon);
            frames_out.push_back(frame_out);
        }

        std::tuple<std::vector<procon::NeoExpandedPolygon>, procon::NeoExpandedPolygon, bool> out = std::make_tuple(frames_out, piece_out, true);
        return out;
    }
    return out_empty;
}

std::pair<polygon_i, bool> PolygonConnector::rotate(polygon_i polygon_in, complex_d after_angle, int first_index, int second_index)
{
    // p = past, poss = possibility

    int point = 0, a, ap, b, bp, n;
    std::vector<complex_d> poss_anglesp;
    polygon_i polygon;
    std::vector<point_i> points, pointsp, points_true;
    std::vector<std::vector<point_i> > points_possiesp;

    auto rotate_point = [](int a, int b, int n, std::vector<point_i> points) //座標回転
    {
        points.push_back({b, a});
        points.push_back({a, -b});
        points.push_back({-b, -a});
        points.push_back({-a, b});

        if(abs(a) > abs(b)) { // |a| は |b| 以下
            int x = a;
            a = b;
            b = x;
        }

        for(int aa = 0; aa < a; ++aa) { //回転
            int B = sqrt(static_cast<double> (n - aa * aa));
            if(B == static_cast<double> (static_cast<int> (B))) { // √(n - a^2) は整数
                int bb = static_cast<int> (B);
                points.push_back({aa, bb});
                points.push_back({bb, aa});
                points.push_back({bb, -aa});
                points.push_back({aa, -bb});
                points.push_back({-aa, -bb});
                points.push_back({-bb, -aa});
                points.push_back({-bb, aa});
                points.push_back({-aa, bb});
            }
        }
        return points;
    };

    auto check_angle_size1 = [&points_possiesp, &poss_anglesp](complex_d i, complex_d ip, std::vector<point_i> points, std::vector<point_i> pointsp) //２点回転
    {
        std::vector<point_i> points_poss;

        for(point_i pointp : pointsp) { //回転前の角度
            complex_d iip(pointp.x(), pointp.y());
            complex_d ip_angle = ip / iip;

            for(point_i point : points) { //回転後の角度
                complex_d ii(point.x(), point.y());
                complex_d i_angle = i / ii;

                if(ip_angle == i_angle) { //角度一致
                    points_poss.push_back(pointp);
                    points_poss.push_back(point);
                    points_possiesp.push_back(points_poss);

                    //引継ぎ
                    poss_anglesp.push_back(ip_angle);

                    points_poss.clear();
                }
            }
        }
    };

    auto check_angle = [&points_possiesp, &poss_anglesp](complex_d i, std::vector<point_i> points) //３点以上回転
    {
        int poss = 0;
        std::vector<complex_d> poss_angles;
        std::vector<std::vector<point_i> > points_possies;

        for(complex_d ip_angle : poss_anglesp) { //回転前の角度
            for(point_i point : points) { //回転後の角度
                complex_d ii(point.x(), point.y());
                complex_d i_angle = i / ii;

                if(ip_angle == i_angle) { //角度一致
                    std::vector<point_i> points_poss = points_possiesp[poss];
                    points_poss.push_back(point);
                    points_possies.push_back(points_poss);

                    //引継ぎ
                    poss_angles.push_back(ip_angle);
                    points_poss.clear();
                }
            }
            ++poss;
        }

        //引継ぎ
        points_possiesp = points_possies;
        poss_anglesp = poss_angles;

        points_possies.clear();
    };

    auto check_return = [&first_index, &second_index, &after_angle, &polygon](std::vector<point_i> points)
    {
        point_i first_point = points.at(first_index), second_point = points.at(second_index);
        complex_d before_angle(static_cast<double> (second_point.x() - first_point.x()), static_cast<double> (second_point.y() - first_point.y()));
        if(before_angle == after_angle) {
            for(point_i point : points) {
                std::vector<point_i> &outer = polygon.outer();
                outer.push_back(point);
            }
            return true;
        } else return false;
    };

    for(int z = 1; z < 4; ++z) { // 絶対にグリッドに乗る3つ（初期polygonはいらない）
        for(point_i polygon_point : polygon_in.outer()) {
            switch (z) {
            case 1:
                points_true.push_back({polygon_point.y(), -polygon_point.x()});
                break;
            case 2:
                points_true.push_back({-polygon_point.x(), -polygon_point.y()});
                break;
            case 3:
                points_true.push_back({-polygon_point.y(), polygon_point.x()});
                break;
            }
        }

        if(check_return(points_true)) {
            std::pair<polygon_i, bool> out = std::make_pair(polygon, true);
            return out;
        }
    }

    for(point_i polygon_point : polygon_in.outer()) { //回転司令塔
        a = polygon_point.x();
        b = polygon_point.y();
        n = a * a + b * b;

        points = rotate_point(a, b, n, points); //今の点

        if(point != 0) {

            //今の点
            complex_d i(a, b);
            complex_d ip(ap, bp);

            (point == 1) ? check_angle_size1(i, ip, points, pointsp) : check_angle(i, points);
        }
        ++point;

        //引継ぎ
        ap = a;
        bp = b;
        pointsp = points;

        points.clear();
    }

    for(std::vector<point_i> points_possp : points_possiesp) {
        if(check_return(points_possp)) {
            std::pair<polygon_i, bool> out = std::make_pair(polygon, true);
            return out;
        }
    }

    std::pair<polygon_i, bool> out = std::make_pair(polygon, false);
    return out;
}
