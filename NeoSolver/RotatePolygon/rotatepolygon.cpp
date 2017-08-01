#include "rotatepolygon.h"

std::vector<polygon_i> rotate_polygon(polygon_i polygon_in)
{
    // p = past 、poss = possibility

    std::vector<polygon_i> polygon_out;

    auto rotate = [&polygon_out, &polygon_in]()
    {
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

        for(int z = 0; z < 4; ++z) { // 絶対にグリッドに乗る４つ
            for(point_i polygon_point : polygon_in.outer()) {
                switch (z) {
                case 0:
                    points_true.push_back({polygon_point.x(), polygon_point.y()});
                    break;
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

            //値のセット
            for(point_i point : points_true) {
                std::vector<point_i> &outer = polygon.outer();
                outer.push_back(point);
            }
            polygon_out.push_back(polygon);

            points_true.clear();
            polygon.clear();
        }

        for(std::vector<point_i> points_possp : points_possiesp) { //回転後の値のセット
            for(point_i pointp : points_possp) {
                std::vector<point_i> &outer = polygon.outer();
                outer.push_back(pointp);
            }
            polygon_out.push_back(polygon);

            polygon.clear();
        }
    };

    auto translate_0 = [](std::vector<polygon_i> polygons)
    {
        for(polygon_i &polygon : polygons) {
            std::vector<int> xs, ys;

            for(point_i point : polygon.outer()) {
                xs.push_back(point.x());
                ys.push_back(point.y());
            }

            int x_min = *std::min_element(xs.begin(), xs.end());
            int y_min = *std::min_element(ys.begin(), ys.end());

            for(point_i &point : polygon.outer()) {
                point = {point.x() - x_min, point.y() - y_min};
            }
        }
        return polygons;
    };

    auto clearing = [](std::vector<polygon_i> polygons)
    {
        std::vector<int> ls;
        std::vector<polygon_i> polygon_out;

        auto check = [&ls](int i)
        {
            for(int j : ls) {
                if(i == j) return true;
            }
            return false;
        };

        for(int i = 0; i != polygons.size() - 1; ++i) {
            if(!check(i)) {
                for(int j = i + 1; j != polygons.size(); ++j) {
                    if(!check(j)) {
                        polygon_i polygon_re = polygons.at(j);
                        bg::reverse(polygon_re);
                        if(bg::equals(polygons.at(i), polygons.at(j)) || bg::equals(polygons.at(i), polygon_re)) {
                            ls.push_back(j);
                        }
                    }
                }
                polygon_out.push_back(polygons.at(i));
            }
        }
        return polygon_out;
    };

    rotate(); //回転

    for(point_i &polygon_point : polygon_in.outer()) { //X軸でひっくり返して
        polygon_point = {polygon_point.x(), -polygon_point.y()};
    }
    rotate(); //回転

    polygon_out = translate_0(polygon_out); //polygonの左下が原点に来るように移動

    polygon_out = clearing(polygon_out); //重複するpolygonを削除

    return polygon_out;
}
