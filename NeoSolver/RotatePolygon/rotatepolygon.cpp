#include "rotatepolygon.h"

std::vector<polygon_i> rotate_polygon(polygon_i polygon_in)
{
    // p = past, poss = possibility

    std::vector<point_i> in_outer = polygon_in.outer();
    std::vector<polygon_i> polygon_out;

    auto rotate = [&polygon_out, &in_outer]()
    {
        int a, ap, b, bp, n;
        int in_size = in_outer.size();
        std::vector<ComplexAngle> poss_anglesp;
        polygon_i polygon;
        std::vector<point_i> points, pointsp;
        std::vector<std::vector<point_i> > points_possiesp;

        auto rotate_point = [&n, &points]() //座標回転
        {
            int x = static_cast<int> (sqrt(static_cast<double> (n) / 2.0)) + 1;
            for(int aa = 0; aa < x; ++aa) { //回転
                double B = sqrt(static_cast<double> (n - aa * aa));
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
        };

        auto check_angle_first = [&points_possiesp, &poss_anglesp](ComplexAngle i, ComplexAngle ip, std::vector<point_i> points, std::vector<point_i> pointsp) //２点回転
        {
            std::vector<point_i> points_poss;

            for(point_i pointp : pointsp) { //前の回転角
                ComplexAngle iip(pointp.x(), pointp.y());
                ComplexAngle ip_angle = ip / iip;

                for(point_i point : points) { //今の回転角
                    ComplexAngle ii(point.x(), point.y());
                    ComplexAngle i_angle = i / ii;

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

        auto check_angle = [&points_possiesp, &poss_anglesp](ComplexAngle i, std::vector<point_i> points) //３点以上回転
        {
            int poss = 0;
            std::vector<ComplexAngle> poss_angles;
            std::vector<std::vector<point_i> > points_possies;

            for(ComplexAngle ip_angle : poss_anglesp) { //前の回転角
                for(point_i point : points) { //今の回転角
                    ComplexAngle ii(point.x(), point.y());
                    ComplexAngle i_angle = i / ii;

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

        for(int j = 1; j < in_size - 1; ++j) { //回転司令塔
            point_i in_point = in_outer.at(j);
            a = in_point.x();
            b = in_point.y();
            n = a * a + b * b;

            rotate_point(); //今の点

            if(j != 1) {
                //今の点
                ComplexAngle i(a, b);
                ComplexAngle ip(ap, bp);

                (j == 2) ? check_angle_first(i, ip, points, pointsp) : check_angle(i, points);
            }

            //引継ぎ
            ap = a;
            bp = b;
            pointsp = points;

            points.clear();
        }

        for(std::vector<point_i> points_possp : points_possiesp) { //回転後の値のセット
            std::vector<point_i> &outer = polygon.outer();

            outer.push_back({0, 0});
            for(point_i pointp : points_possp) {
                outer.push_back(pointp);
            }
            outer.push_back({0, 0});

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

        int polygons_size = polygons.size();
        for(int i = 0; i < polygons_size - 1; ++i) {
            if(!check(i)) {
                for(int j = i + 1; j < polygons_size; ++j) {
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

    point_i in_point0 = in_outer.at(0);
    for(point_i &in_point : in_outer) {
        in_point = {in_point.x() - in_point0.x(), in_point.y() - in_point0.y()};
    }

    rotate(); //回転

    for(point_i &in_point : in_outer) { //X軸でひっくり返して
        in_point = {in_point.x(), -in_point.y()};
    }
    rotate(); //回転

    polygon_out = translate_0(polygon_out); //polygonの左下が原点に来るように移動

    polygon_out = clearing(polygon_out); //重複するpolygonを削除

    return polygon_out;
}
