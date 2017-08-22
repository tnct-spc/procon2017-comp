#include "polygonconnector.h"
#include <boost/geometry/algorithms/union.hpp>
#define DEBUG
#define TEST
//#define USE_BOOST_TRANSFORM

PolygonConnector::PolygonConnector()
{

}

std::tuple<std::vector<procon::NeoExpandedPolygon>, procon::NeoExpandedPolygon, bool> PolygonConnector::connect(procon::NeoExpandedPolygon frame, procon::NeoExpandedPolygon piece, Connect connecter)
#ifdef USE_BOOST_TRANSFORM
{
    //Connectクラスからindexを抽出
    int frame_index1 = connecter.frame_point_index; //フレームの接すべき点のindex
    int piece_index1 = connecter.polygon_point_index; //ピースの接すべき点のindex
    int frame_index2; //フレームのもう一方の点のindex
    int piece_index2; //ピースのもう一方の点のindex
    if(connecter.frame_point_index == connecter.frame_side_index) { //接すべき点のindex < もう一方の点のindex
        //indexが参照外にならないように
        frame_index2 = (frame_index1 != frame.getSize() - 1) ? frame_index1 + 1 : 0;
        piece_index2 = (piece_index1 != piece.getSize() - 1) ? piece_index1 + 1 : 0;
    } else { //接すべき点のindex > もう一方の点のindex
        //indexが参照外にならないように
        frame_index2 = (frame_index1 != 0) ? frame_index1 - 1 : frame.getSize() - 1;
        piece_index2 = (piece_index1 != 0) ? piece_index1 - 1 : piece.getSize() - 1;
    }

#ifndef DEBUG
    //駄目だった場合に返すやつ
    const std::vector<procon::NeoExpandedPolygon> empty_frame;
    const procon::NeoExpandedPolygon empty_piece;
    const std::tuple<std::vector<procon::NeoExpandedPolygon>, procon::NeoExpandedPolygon, bool> out_empty = std::make_tuple(empty_frame, empty_piece, false);
#endif

    //フレームの辺の傾き
    polygon_i frame_polygon = frame.getPolygon();
    std::vector<point_i> frame_outer = frame_polygon.outer();
    point_i frame_point1 = frame_outer.at(frame_index1);
    point_i frame_point2 = frame_outer.at(frame_index2);
    int x2 = frame_point2.x() - frame_point1.x();
    int y2 = frame_point2.y() - frame_point1.y();
    ComplexAngle frame_angle(x2, y2); //移動後の傾き

    //ピースの辺の傾き
    polygon_i piece_polygon = piece.getPolygon();
    std::vector<point_i> piece_outer = piece_polygon.outer();
    point_i piece_point1 = piece_outer.at(piece_index1);
    point_i piece_point2 = piece_outer.at(piece_index2);
    int x1 = piece_point2.x() - piece_point1.x();
    int y1 = piece_point2.y() - piece_point1.y();
    ComplexAngle piece_angle(x1, y1); //移動前の傾き

    ComplexAngle angle = frame_angle / piece_angle; //回転角
    double atan = std::atan2(angle.y, angle.x);
    if(std::signbit(atan)) atan += M_PI * 2;
    //else if (std::signbit(angle.x)) atan += M_PI;

    point_i piece_rotate_point1;
    polygon_i piece_rotate_polygon;
    polygon_t piece_rotate_polygon_t;
    std::vector<point_i> piece_rotate_outer;

    if(atan == 0.0) {
        piece_rotate_point1 = piece_point1;
        piece_rotate_polygon = piece_polygon;
        piece_rotate_outer = piece_outer;
    } else {
        for(point_i &piece_point : piece_polygon.outer()) {
            piece_point = {piece_point.x() - piece_point1.x(), piece_point.y() - piece_point1.y()};
        }
        piece.resetPolygonForce(piece_polygon);

        bg::strategy::transform::rotate_transformer<bg::radian, double, 2, 2> translate(-atan);
        bg::transform(piece_polygon, piece_rotate_polygon_t, translate);

        for(point_t &piece_point : piece_rotate_polygon_t.outer()) {
            piece_point = {piece_point.x() + static_cast<double> (frame_point1.x()), piece_point.y() + static_cast<double> (frame_point1.y())};
        }

        auto check_int = [&piece_rotate_polygon_t]()
        {
            int i = 0;
            int j = 0;
            for(point_t &piece_rotate_point_t : piece_rotate_polygon_t.outer()) {
                double x = piece_rotate_point_t.x();
                double y = piece_rotate_point_t.y();
                double xint, xdouble, yint, ydouble;
                xdouble = std::modf(x, &xint);
                ydouble = std::modf(y, &yint);
                if(xdouble > 0.999990) {
                    xint += 1.0;
                    xdouble = 0.0;
                    x = xint + xdouble;
                }
                if(ydouble > 0.999990) {
                    yint += 1.0;
                    ydouble = 0.0;
                    y = yint + ydouble;
                }
                piece_rotate_point_t = {x, y};
                if((x == xint) && (y == yint)) ++j;
                ++i;
            }
            return (i == j) ? true : false;
        };

        if(!check_int()) {
#ifdef DEBUG
            auto out_t = [](polygon_t poly, std::string name)
            {
                std::cout << name << " -> ";

                for(point_t point : poly.outer()) {
                    std::cout << "(" << point.x() << ", " << point.y() << "), ";
                }

                std::cout << std::endl << std::endl;
            };
            out_t(piece_rotate_polygon_t, "piece_t");

            piece.translatePolygon(frame_point1.x(), frame_point1.y());
            std::vector<procon::NeoExpandedPolygon> frames;
            frames.push_back(frame);
            const std::tuple<std::vector<procon::NeoExpandedPolygon>, procon::NeoExpandedPolygon, bool> out_debug0 = std::make_tuple(frames, piece, false);
            return out_debug0;
#endif
#ifndef DEBUG
            return out_empty;
#endif
        }
#ifdef TEST
        else std::cout << "check_int is true!" << std::endl;
#endif

        std::vector<point_i> &_piece_rotate_outer = piece_rotate_polygon.outer();
        for(point_t &piece_rotate_point_t : piece_rotate_polygon_t.outer()) {
            _piece_rotate_outer.push_back({static_cast<int> (piece_rotate_point_t.x()), static_cast<int> (piece_rotate_point_t.y())});
        }
        piece_rotate_outer = piece_rotate_polygon.outer();
        point_i point_out = piece_rotate_outer.at(1);
        if((point_out.x() == 0) && (point_out.y() == 0)) {
#ifdef DEBUG
            piece.translatePolygon(frame_point1.x(), frame_point1.y());
            std::vector<procon::NeoExpandedPolygon> frames;
            frames.push_back(frame);
            const std::tuple<std::vector<procon::NeoExpandedPolygon>, procon::NeoExpandedPolygon, bool> out_debug1 = std::make_tuple(frames, piece, false);
            return out_debug1;
#endif
#ifndef DEBUG
            return out_empty;
#endif
        }
#ifdef TEST
        else std::cout << "rotate is true!" << std::endl;
#endif
        piece_rotate_point1 = piece_rotate_outer.at(piece_index1);
    }
    procon::NeoExpandedPolygon piece_out(piece.getId());
    piece_out.resetPolygonForce(piece_rotate_polygon);
    std::vector<polygon_i> union_polygons;
    bg::union_(frame_polygon, piece_rotate_polygon, union_polygons);
    if(union_polygons.size() != 1) {
#ifdef TEST
        auto out = [](polygon_i poly, std::string name)
        {
            std::cout << name << " -> ";

            for(point_i point : poly.outer()) {
                std::cout << "(" << point.x() << ", " << point.y() << "), ";
            }

            std::cout << std::endl << std::endl;
        };
        out(union_polygons.at(0), "union1");
        out(union_polygons.at(1), "union2");
#endif
#ifdef DEBUG
        std::vector<procon::NeoExpandedPolygon> frames;
        frames.push_back(frame);
        const std::tuple<std::vector<procon::NeoExpandedPolygon>, procon::NeoExpandedPolygon, bool> out_debug2 = std::make_tuple(frames, piece_out, false);
        return out_debug2;
#endif
#ifndef DEBUG
        return out_empty;
#endif
    }
#ifdef TEST
    else std::cout << "union_polygon is one!" << std::endl;
#endif
    polygon_i union_polygon = union_polygons.at(0);

    if(bg::equals(frame_polygon, union_polygon)) {
        std::vector<polygon_i> frame_out_polygons;
#ifdef TEST
        std::cout << "equals is true!" << std::endl;
#endif
        bg::difference(frame_polygon, piece_rotate_polygon, frame_out_polygons);
        std::vector<procon::NeoExpandedPolygon> frames_out;

        int id= 0;
        for(polygon_i frame_out_polygon : frame_out_polygons) {
            procon::NeoExpandedPolygon frame_out(id);
            frame_out.resetPolygonForce(frame_out_polygon);
            frames_out.push_back(frame_out);
            ++id;
        }

        std::tuple<std::vector<procon::NeoExpandedPolygon>, procon::NeoExpandedPolygon, bool> out = std::make_tuple(frames_out, piece_out, true);
        return out;
    }
#ifdef DEBUG
    std::vector<procon::NeoExpandedPolygon> frames;
    frames.push_back(frame);
    const std::tuple<std::vector<procon::NeoExpandedPolygon>, procon::NeoExpandedPolygon, bool> out_debug3 = std::make_tuple(frames, piece_out, false);
    return out_debug3;
#endif
#ifndef DEBUG
        return out_empty;
#endif
}
#endif

#ifndef USE_BOOST_TRANSFORM
//フレームにピースをはめる関数
//回転関数により、ピースを原点を中心に回転させたとき、全ての点が整数値の座標を持つようなピースを抽出する。
//重なるべき辺において、抽出されたピースの辺の傾きとはめるフレームの辺の傾きが合致しているか、調べる。
//角度が合致していた場合、ピースがフレームに重なるように、ピースを移動する。
//ピースがフレームからはみ出してないか調べるため、ピースとフレームを足し合わせたものと、元々のフレームが同じ形かを調べる。
//はみ出していなければ、フレームからピースを引いたものが、フレームにピースをはめた後のフレームとなる。

{
    //Connectクラスからindexを抽出
    int frame_index1 = connecter.frame_point_index; //フレームの接すべき点のindex
    int piece_index1 = connecter.polygon_point_index; //ピースの接すべき点のindex
    int frame_index2; //フレームのもう一方の点のindex
    int piece_index2; //ピースのもう一方の点のindex
    if(connecter.frame_point_index == connecter.frame_side_index) { //接すべき点のindex < もう一方の点のindex
        //indexが参照外にならないように
        frame_index2 = (frame_index1 != frame.getSize() - 1) ? frame_index1 + 1 : 0;
        piece_index2 = (piece_index1 != piece.getSize() - 1) ? piece_index1 + 1 : 0;
    } else { //接すべき点のindex > もう一方の点のindex
        //indexが参照外にならないように
        frame_index2 = (frame_index1 != 0) ? frame_index1 - 1 : frame.getSize() - 1;
        piece_index2 = (piece_index1 != 0) ? piece_index1 - 1 : piece.getSize() - 1;
    }

    //駄目だった場合に返すやつ
    const std::vector<procon::NeoExpandedPolygon> empty_frame;
    const procon::NeoExpandedPolygon empty_piece;
    const std::tuple<std::vector<procon::NeoExpandedPolygon>, procon::NeoExpandedPolygon, bool> out_empty = std::make_tuple(empty_frame, empty_piece, false);

    //はめるフレームの辺の傾きを算出
    polygon_i frame_polygon = frame.getPolygon();
    std::vector<point_i> frame_outer = frame_polygon.outer();
    point_i frame_point1 = frame_outer.at(frame_index1);
    point_i frame_point2 = frame_outer.at(frame_index2);
    int x2 = frame_point2.x() - frame_point1.x(); //移動後の角度だから x2, y2
    int y2 = frame_point2.y() - frame_point1.y(); //移動前(piece)の角度はrotateで算出
    point_i frame_angle(x2, y2); //移動後の角度

    polygon_i piece_polygon = piece.getPolygon();
    std::pair<polygon_i, bool> rotate_out = rotate(piece_polygon, frame_angle, piece_index1, piece_index2);
    if(!rotate_out.second) {
#ifdef DEBUG
        std::vector<point_i> piece_outer = piece_polygon.outer();
        point_i piece_point1 = piece_outer.at(piece_index1);
        piece.translatePolygon(frame_point1.x() - piece_point1.x(), frame_point1.y() - piece_point1.y());
        const std::tuple<std::vector<procon::NeoExpandedPolygon>, procon::NeoExpandedPolygon, bool> out_debug1 = std::make_tuple(empty_frame, piece, false);
        return out_debug1;
#endif
#ifndef DEBUG
        return out_empty;
#endif
    }
    //else std::cout << "rotate is true!" << std::endl;
    polygon_i piece_out_polygon = rotate_out.first;
    std::vector<point_i> piece_out_points = piece_out_polygon.outer();
    point_i piece_out_point1 = piece_out_points.at(piece_index1);

    procon::NeoExpandedPolygon piece_out(piece.getId());
    piece_out.resetPolygonForce(piece_out_polygon);
    piece_out.translatePolygon(frame_point1.x() - piece_out_point1.x(), frame_point1.y() - piece_out_point1.y());
    piece_out_polygon = piece_out.getPolygon();
    std::vector<polygon_i> union_polygons;
    bg::union_(frame_polygon, piece_out_polygon, union_polygons);
    if(union_polygons.size() != 1) {
//        auto out = [](polygon_i poly, std::string name)
//        {
//            std::cout << name << " -> ";

//            for(point_i point : poly.outer()) {
//                std::cout << "(" << point.x() << ", " << point.y() << "), ";
//            }

//            std::cout << std::endl << std::endl;
//        };
//        out(union_polygons.at(0), "union1");
//        out(union_polygons.at(1), "union2");
#ifdef DEBUG
        const std::tuple<std::vector<procon::NeoExpandedPolygon>, procon::NeoExpandedPolygon, bool> out_debug2 = std::make_tuple(empty_frame, piece_out, false);
        return out_debug2;
#endif
#ifndef DEBUG
        return out_empty;
#endif
    } //else std::cout << "union_polygon is one!" << std::endl;
    polygon_i union_polygon = union_polygons.at(0);

    if(bg::equals(frame_polygon, union_polygon)) {
        std::vector<polygon_i> frame_out_polygons;
        //std::cout << "equals is true!" << std::endl;
        bg::difference(frame_polygon, piece_out_polygon, frame_out_polygons);
        std::vector<procon::NeoExpandedPolygon> frames_out;

        int id= 0;
        for(polygon_i frame_out_polygon : frame_out_polygons) {
            procon::NeoExpandedPolygon frame_out(id);
            frame_out.resetPolygonForce(frame_out_polygon);
            frames_out.push_back(frame_out);
            ++id;
        }

        std::tuple<std::vector<procon::NeoExpandedPolygon>, procon::NeoExpandedPolygon, bool> out = std::make_tuple(frames_out, piece_out, true);
        return out;
    }
#ifdef DEBUG
        const std::tuple<std::vector<procon::NeoExpandedPolygon>, procon::NeoExpandedPolygon, bool> out_debug3 = std::make_tuple(empty_frame, piece_out, false);
        return out_debug3;
#endif
#ifndef DEBUG
        return out_empty;
#endif
}

std::pair<polygon_i, bool> PolygonConnector::rotate(polygon_i polygon_in, point_i after_angle, int first_index, int second_index)
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
        double x1 = second_point.x() - first_point.x();
        double y1 = second_point.y() - first_point.y();
        double x2 = after_angle.x();
        double y2 = after_angle.y();
        if(((y1 == 0) && (y2 == 0)) && (std::signbit(x1) == std::signbit(x2))) {
            for(point_i point : points) {
                std::vector<point_i> &outer = polygon.outer();
                outer.push_back(point);
            }
            return true;
        } else if((x1 / y1 == x2 / y2) && (std::signbit(x1) == std::signbit(x2)) && (std::signbit(y1) == std::signbit(y2))) {
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
        points_true.clear();
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
#endif
