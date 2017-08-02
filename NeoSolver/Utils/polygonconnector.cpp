#include "polygonconnector.h"

PolygonConnector::PolygonConnector()
{

}

std::pair<std::vector<procon::NeoExpandedPolygon>, procon::NeoExpandedPolygon> PolygonConnector::connect(procon::NeoExpandedPolygon frame, procon::NeoExpandedPolygon piece, Connect connecter)
{
    int frame_index1 = connecter.frame_point_index, piece_index1 = connecter.polygon_point_index;
    int frame_index2, piece_index2;

    if(connecter.frame_point_index == connecter.frame_side_index) {
        frame_index2 = (frame_index1 != frame.getSize() - 1) ? frame_index1 + 1 : 0;
        piece_index2 = (piece_index1 != piece.getSize() - 1) ? piece_index1 + 1 : 0;
    } else {f
        frame_index2 = (frame_index1 != 0) ? frame_index1 - 1 : frame.getSize() - 1;
        piece_index2 = (piece_index1 != 0) ? piece_index1 - 1 : piece.getSize() - 1;
    }

    polygon_i frame_polygon = frame.getPolygon(), piece_polygon = piece.getPolygon(), union_polygon;
    std::vector<polygon_i> frame_out_polygons;
    std::vector<point_i> frame_points = frame_polygon.outer();
    point_i frame_point1 = frame_points.at(frame_index1), frame_point2 = frame_points.at(frame_index2);
    complex_d frame_complex(static_cast<double> (frame_point2.x() - frame_point1.x()), static_cast<double> (frame_point2.y() - frame_point1.y()));

    polygon_i piece_out_polygon = (this -> rotate(piece_polygon, frame_complex, piece_index1, piece_index2));
    std::vector<point_i> piece_out_points = piece_out_polygon.outer();
    point_i piece_out_point1 = piece_out_points.at(piece_index1);

    procon::NeoExpandedPolygon piece_out;
    piece_out.resetPolygonForce(piece_out_polygon);
    piece_out.translatePolygon(piece_out_point1.x(), piece_out_point1.y());
    bg::union_(frame_polygon, piece_out_polygon, union_polygon);

    if(bg::equals(frame_polygon, union_polygon)) {
        bg::difference(frame_polygon, piece_out_polygon, frame_out_polygons);
        std::vector<procon::NeoExpandedPolygon> frames_out;

        for(polygon_i frame_out_polygon : frame_out_polygons) {
            procon::NeoExpandedPolygon frame_out;
            frame_out.resetPolygonForce(frame_out_polygon);
            frames_out.push_back(frame_out);
        }

        std::pair<std::vector<procon::NeoExpandedPolygon>, procon::NeoExpandedPolygon> out = std::make_pair(frames_out, piece_out);
        return out;
    }
    return NULL;
}

polygon_i PolygonConnector::rotate(polygon_i polygon_in, complex_d after_angle, int first_index, int second_index)
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

    auto check_return = [](std::vector<point_i> points)
    {
        point_i first_point = points.at(first_index), second_point = points.at(second_index);
        complex_d before_angle(static_cast<double> (second_point.x() - first_point.x()), static_cast<double> (second_point.y() - first_point.y()));
        return (before_angle == after_angle) ? true : false;
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
            for(point_i point : points_true) {
                std::vector<point_i> &outer = polygon.outer();
                outer.push_back(point);
            }
            return polygon;
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
            for(point_i point : points_possp) {
                std::vector<point_i> &outer = polygon.outer();
                outer.push_back(point);
            }
            return polygon;
        }
    }

    return NULL;
}
