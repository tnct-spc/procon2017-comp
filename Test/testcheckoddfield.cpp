#include "testcheckoddfield.h"

int checkOddField(const procon::NeoField &field, const Connect &connector, const int field_frame_index, const int field_piece_index) //field_frame_indexはConnectクラスのindexで参照すべきframeのindex
{
    //mag = magnification（倍率）
    int odd_score = 0; //ゲテモノポイント
    const int odd_connect_length = 3; //これ以下の長さだとゲテモノ（単位はグリット）
    const int check_connect_length_mag = 10; //１倍のときのゲテモノポイント加算値はodd_connect_length以下の整数になる

    std::vector<procon::NeoExpandedPolygon> frames = field.getFrame();

    auto check_connect_length = [&]() //結合した辺のゲテモノ度
    {
        procon::NeoExpandedPolygon frame = frames.at(static_cast<unsigned int>(field_frame_index));
        std::vector<double> frame_lengthes = frame.getSideLength();
        double frame_length = frame_lengthes.at(static_cast<unsigned int>(connector.frame_side_index));

        auto checklength = [](double length)
        {
            double length_int;
            double length_double = modf(length, &length_int);
            return static_cast<int>((length_double > 0.0) ? length_int + 1.0 : length_int);
        };

        int frame_length_up_int = checklength(frame_length);
        if(frame_length_up_int <= odd_connect_length) odd_score += (odd_connect_length + 1 - frame_length_up_int) * check_connect_length_mag;
        else {
            procon::NeoExpandedPolygon piece = field.getPiece(field_piece_index);
            std::vector<double> piece_lengthes = piece.getSideLength();
            double piece_length = piece_lengthes.at(static_cast<unsigned int>(connector.polygon_side_index));
            int piece_length_up_int = checklength(piece_length);
            if(piece_length_up_int <= odd_connect_length) odd_score += (odd_connect_length + 1 - piece_length_up_int) * check_connect_length_mag;
        }
    };

    if(odd_connect_length > 0 && check_connect_length_mag > 0) check_connect_length();

    return odd_score;
}

testCheckOddField::testCheckOddField()
{

}

bool testCheckOddField::run()
{
    std::vector<polygon_i> frame_polygons{{{{0, 0}, {0, 50}, {30, 50}, {35, 43}, {38, 43}, {45, 50}, {100, 50}, {100, 0}, {0, 0}}}
                                         };
    std::vector<polygon_i> piece_polygons{{{{35, 43}, {38, 43}, {50, 38}, {43, 20}, {35, 43}}},
                                          {{{43, 20}, {35, 43}, {62, 43}, {50, 38}, {43, 20}}}
                                         };
    Connect connecter;
    int frame_side, piece_side, frame_point, piece_point;
    int i = 0, j = 0, odd_score = 0;
    for(polygon_i frame_polygon : frame_polygons) {
        procon::NeoExpandedPolygon frame;
        frame.resetPolygonForce(frame_polygon);
        std::vector<procon::NeoExpandedPolygon> frames;
        frames.push_back(frame);

        auto out = [](procon::NeoExpandedPolygon expoly, std::string name)
        {
            polygon_i poly = expoly.getPolygon();

            std::cout << name << " -> ";

            for(point_i point : poly.outer()) {
                std::cout << "(" << point.x() << ", " << point.y() << "), ";
            }

            std::cout << std::endl;

            NeoPolygonViewer::getInstance().displayPolygon(poly, name, false);
        };

        out(frame, "frame < " + std::to_string(i) + " >");

        for(polygon_i piece_polygon : piece_polygons) {
            if(i == 0) {
                if(j == 0) {
                    piece_side = 0;
                    piece_point = 0;
                } else if(j == 1) {
                    piece_side = 1;
                    piece_point = 1;
                } else {
                    piece_side = -1;
                }
                frame_side = 3;
                frame_point = 3;
            }

            if(piece_side != -1) {
                procon::NeoExpandedPolygon piece;
                piece.resetPolygonForce(piece_polygon);
                procon::NeoField field;
                field.setFrame(frames);
                field.setPiece(piece);

                connecter.frame_side_index = frame_side;
                connecter.polygon_side_index = piece_side;
                connecter.frame_point_index = frame_point;
                connecter.polygon_point_index = piece_point;

                odd_score = checkOddField(field, connecter, 0, 0);
                out(piece, "piece < " + std::to_string(j) + " >");
                std::cout << "odd_score = " << odd_score << std::endl;
            }
            ++j;
        }
        ++i;
    }
    return  true;
}
