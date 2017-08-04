#include "testpolygonconnector.h"

TestPolygonConnector::TestPolygonConnector()
{

}

bool TestPolygonConnector::run()
{
    std::vector<polygon_i> polygon_frames{{{{1, 4}, {0, 5}, {2, 6}, {1, 9}, {1, 10}, {4, 10}, {5, 8}, {4, 5}, {5, 2}, {4, 2}, {3, 5}, {2, 3}, {3, 2}, {1, 2}, {1, 4}}}
                                        };
    std::vector<polygon_i> polygon_pieces{{{{6, 1}, {8, 2}, {8, 1}, {6, 1}}},
                                         {{{10, 3}, {8, 3}, {8, 5}, {10, 3}}},
                                         {{{2, 15}, {4, 16}, {4, 13}, {2, 12}, {2, 15}}},
                                         {{{6, 9}, {7, 12}, {9, 11}, {10, 8}, {10, 7}, {7, 7}, {6, 9}}}
                                        };
    procon::NeoExpandedPolygon frame;
    procon::NeoExpandedPolygon piece;
    Connect connecter;
    int frame_side, piece_side, frame_point, piece_point;
    PolygonConnector polygon_connector;


    for(int i = 0; i < polygon_frames.size(); ++i) {
        std::cout << std::endl << std::endl << "--frame---------------------------<< " << i << " >>---------------------------" << std::endl << std::endl;
        polygon_i polygon_frame = polygon_frames.at(i);

        for(int j = 0; j < polygon_pieces.size(); ++j) {
            std::cout << std::endl << "--piece---------------------------<< " << j << " >>---------------------------" << std::endl << std::endl;
            polygon_i polygon_piece = polygon_pieces.at(j);

            frame.resetPolygonForce(polygon_frame);
            piece.resetPolygonForce(polygon_piece);

            if(i == 0) {
                if(j == 0) {
                    frame_side = 5;
                    piece_side = 0;
                    piece_point = 0;
                } else if(i == 1) {
                    frame_side = 4;
                    piece_side = 0;
                    piece_point = 1;
                } else if(i == 2) {
                    frame_side = 4;
                    piece_side = 1;
                    piece_point = 2;
                } else if(i == 3) {
                    frame_side = 4;
                    piece_side = 4;
                    piece_point = 5;
                }
                frame_point = 5;
            }

            connecter.frame_side_index = frame_side;
            connecter.polygon_side_index = piece_side;
            connecter.frame_point_index = frame_point;
            connecter.polygon_point_index = piece_point;

            std::tuple<std::vector<procon::NeoExpandedPolygon>, procon::NeoExpandedPolygon, bool> result = polygon_connector.connect(frame, piece, connecter);
            if(std::get<2>(result)) {
                std::vector<procon::NeoExpandedPolygon> result_frames = std::get<0>(result);
                procon::NeoExpandedPolygon result_piese = std::get<1>(result);

                auto out = [](procon::NeoExpandedPolygon expoly, std::string name)
                {
                    polygon_i poly = expoly.getPolygon();

                    std::cout << name << " -> ";

                    for(point_i point : poly.outer()) {
                        std::cout << "(" << point.x() << ", " << point.y() << "), ";
                    }

                    std::cout << std::endl << std::endl;
                };

                for(procon::NeoExpandedPolygon result_frame : result_frames) {
                    out(result_frame, "frame");
                }
                std::cout << std::endl;
                out(result_piese, "piese");
            } else std::cout << "false" << std::endl;
            std::cout << "end" << std::endl;
        }
    }
    return true;
}
