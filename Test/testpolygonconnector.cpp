#include "testpolygonconnector.h"
#include "neofield.h"

TestPolygonConnector::TestPolygonConnector()
{

}

bool TestPolygonConnector::run()
{
    std::vector<std::vector<polygon_i>> some_polygon_frames{{{{{1, 4}, {0, 5}, {2, 6}, {1, 9}, {1, 10}, {4, 10}, {5, 8}, {4, 5}, {5, 2}, {4, 2}, {3, 5}, {2, 3}, {3, 2}, {1, 2}, {1, 4}}}
                                                            },
                                                            {{{{1, 4}, {0, 5}, {2, 6}, {1, 9}, {1, 10}, {4, 10}, {5, 8}, {4, 5}, {5, 2}, {4, 2}, {3, 5}, {2, 3}, {3, 2}, {1, 2}, {1, 4}}}
                                                            }
                                                           };
    std::vector<std::vector<polygon_i>> some_polygon_pieces{{{{{6, 1}, {8, 2}, {8, 1}, {6, 1}}},
                                                             {{{9, 1}, {9, 3}, {10, 3}, {12, 1}, {9, 1}}},
                                                             {{{12, 3}, {13, 5}, {15, 4}, {15, 2}, {12, 3}}},
                                                             {{{7, 6}, {7, 7}, {9, 6}, {8, 5}, {8, 6}, {7, 6}}},
                                                             {{{10, 4}, {10, 5}, {11, 5}, {11, 4}, {10, 4}}},
                                                             {{{6, 3}, {7, 4}, {7, 3}, {6, 3}}},
                                                             {{{11, 6}, {12, 8}, {12, 7}, {11, 6}}},
                                                             {{{15, 7}, {16, 9}, {17, 6}, {17, 5}, {15, 7}}},
                                                             {{{8, 8}, {7, 9}, {10, 9}, {8, 8}}},
                                                             {{{11, 9}, {11, 10}, {14, 11}, {14, 10}, {11, 9}}},
                                                             {{{15, 12}, {17, 12}, {17, 10}, {15, 12}}}
                                                            },
                                                            {{{{6, 1}, {8, 2}, {8, 1}, {6, 1}}},
                                                             {{{9, 1}, {9, 4}, {12, 1}, {9, 1}}},
                                                             {{{12, 3}, {13, 5}, {15, 4}, {15, 2}, {12, 3}}},
                                                             {{{7, 6}, {7, 7}, {9, 6}, {8, 5}, {8, 6}, {7, 6}}},
                                                             {{{10, 4}, {10, 5}, {11, 5}, {11, 4}, {10, 4}}},
                                                             {{{6, 3}, {7, 4}, {7, 3}, {6, 3}}},
                                                             {{{11, 6}, {12, 8}, {12, 7}, {11, 6}}},
                                                             {{{15, 7}, {16, 9}, {17, 6}, {17, 5}, {15, 7}}},
                                                             {{{8, 8}, {7, 9}, {10, 9}, {8, 8}}},
                                                             {{{11, 9}, {11, 10}, {14, 11}, {14, 10}, {11, 9}}},
                                                             {{{15, 12}, {17, 12}, {17, 10}, {15, 12}}}
                                                            }
                                                           };
    std::vector<std::vector<Connect>> some_connectors{{{0, 0, 0, 0}, {0, 3, 0, 4}, {0, 2, 0, 3}},
                                                      {{0, 0, 0, 0}, {0, 2, 0, 3}}
                                                     };
    std::vector<std::vector<std::pair<point_i, point_i>>> some_connect_points{{{{4, 10}, {4, 10}}, {{1, 10}, {4, 10}}, {{3, 8}, {5, 8}}},
                                                                              {{{4, 10}, {4, 10}}, {{1, 10}, {4, 10}}}
                                                                             };
    std::vector<std::vector<polygon_i>> some_polygon_hint_pieces{{{{{2, 5}, {3, 5}, {2, 3}, {1, 4}, {2, 4}, {2, 5}}},
                                                                  {{{1, 4}, {1, 5}, {2, 5}, {2, 4}, {1, 4}}},
                                                                  {{{1, 4}, {0, 5}, {1, 5}, {1, 4}}},
                                                                  {{{2, 6}, {4, 5}, {3, 5}, {2, 6}}},
                                                                  {{{3, 8}, {2, 6}, {1, 9}, {1, 10}, {3, 8}}},
                                                                  {{{3, 5}, {0, 5}, {2, 6}, {3, 5}}},
                                                                  {{{5, 2}, {4, 2}, {3, 5}, {4, 5}, {5, 2}}},
                                                                  {{{3, 2}, {1, 2}, {1, 4}, {3, 2}}},
                                                                 },
                                                                 {{{{4, 5}, {2, 6}, {3, 8}, {5, 8}, {4, 5}}},
                                                                  {{{2, 5}, {3, 5}, {2, 3}, {1, 4}, {2, 4}, {2, 5}}},
                                                                  {{{1, 4}, {1, 5}, {2, 5}, {2, 4}, {1, 4}}},
                                                                  {{{1, 4}, {0, 5}, {1, 5}, {1, 4}}},
                                                                  {{{2, 6}, {4, 5}, {3, 5}, {2, 6}}},
                                                                  {{{3, 8}, {2, 6}, {1, 9}, {1, 10}, {3, 8}}},
                                                                  {{{3, 5}, {0, 5}, {2, 6}, {3, 5}}},
                                                                  {{{5, 2}, {4, 2}, {3, 5}, {4, 5}, {5, 2}}},
                                                                  {{{3, 2}, {1, 2}, {1, 4}, {3, 2}}},
                                                                 }
                                                                };
//    NeoPolygonViewer::getInstance().displayPolygon(some_polygon_frames.at(0).at(0), "pre frame", false);
//    int a = 0;
//    for(polygon_i piece : some_polygon_pieces.at(0)) {
//        NeoPolygonViewer::getInstance().displayPolygon(piece, "pre piece " + std::to_string(a), false);
//        ++a;
//    }
//    a = 0;
//    for(polygon_i hint : some_polygon_hint_pieces.at(0)) {
//        NeoPolygonViewer::getInstance().displayPolygon(hint, "pre hint " + std::to_string(a), false);
//        ++a;
//    }

    unsigned int i = 0;
    for(std::vector<Connect> connectors : some_connectors) {
        std::cout << std::endl << "start connect... (" << i << ")" << std::endl;
        unsigned int j = 0;
        std::vector<procon::NeoExpandedPolygon> frames;
        frames.reserve(100);
        for(polygon_i polygon_frame : some_polygon_frames.at(i)) {
            procon::NeoExpandedPolygon frame(static_cast<int>(j));
            frame.resetPolygonForce(polygon_frame);
            frames.emplace_back(frame);
            ++j;
        }

        std::vector<procon::NeoExpandedPolygon> pieces;
        pieces.reserve(100);
        j = 0;
        for(polygon_i polygon_piece : some_polygon_pieces.at(i)) {
            procon::NeoExpandedPolygon piece(static_cast<int>(j));
            piece.resetPolygonForce(polygon_piece);
            pieces.emplace_back(piece);
            ++j;
        }

        std::vector<procon::NeoExpandedPolygon> hint_pieces;
        hint_pieces.reserve(100);
        if(i == 0) j = 3;
        else if(i == 1) j = 2;
        else j = 0;
        for(polygon_i polygon_hint_piece : some_polygon_hint_pieces.at(i)) {
            procon::NeoExpandedPolygon hint_piece(static_cast<int>(j));
            hint_piece.resetPolygonForce(polygon_hint_piece);
            hint_pieces.emplace_back(hint_piece);
            ++j;
        }

        procon::NeoField field;
        field.setElementaryFrame(frames);
        field.setElementaryPieces(pieces);
        field.setElementaryFrameInnerPieces(hint_pieces);
        field.setFrame(frames);

        auto find_point = [](const std::vector<point_i> &points, const point_i &target)
        {
            int i = 0;
            for(const point_i &point : points) {
                if(point.x() == target.x() && point.y() == target.y()) return i;
                ++i;
            }
        };

        std::cout << std::endl << "now connecting..." << std::endl;

        j = 0;
        for(Connect connector : connectors) {
            std::cout << "connector_num = " << j << std::endl;

            std::vector<point_i> frame_outer = field.getFrame().at(0).getPolygon().outer();
//            for(const procon::NeoExpandedPolygon &now_frame : field.getFrame()) {
//                std::copy(now_frame.getPolygon().outer().begin(),now_frame.getPolygon().outer().end(),std::back_inserter(frame_outer));
//            }
            connector.frame_side_index = find_point(frame_outer, some_connect_points.at(i).at(j).first);
            connector.frame_point_index = find_point(frame_outer, some_connect_points.at(i).at(j).second);

            std::tuple<std::vector<procon::NeoExpandedPolygon>, procon::NeoExpandedPolygon, bool> result =
                    PolygonConnector::connect(field.getFrame().at(0), pieces.at(j), connector);

            if(std::get<2>(result)) {
                field.setFrame(std::get<0>(result));
                field.setPiece(std::get<1>(result));
                field.setIsPlaced(pieces.at(j).getId());
                std::cout << "true" <<std::endl;
            } else {
//                NeoPolygonViewer::getInstance().displayPolygon(std::get<1>(result).getPolygon(), "debug piece " + std::to_string(j), false);
//                NeoPolygonViewer::getInstance().displayPolygon(std::get<0>(result).at(0).getPolygon(), "debug frame " + std::to_string(j), false);
                std::cout << "false" <<std::endl;
            }
            ++j;
        }
        std::cout << std::endl << std::endl << "checking hint..." << std::endl;
        bool check = field.check_hint();
        if(check) {
//            int a = 0;
//            for(procon::NeoExpandedPolygon frame : field.getFrame()) {
//                NeoPolygonViewer::getInstance().displayPolygon(frame.getPolygon(), "frame " + std::to_string(a), false);
//                ++a;
//            }
            std::cout << "true" <<std::endl;
        } else std::cout << "false" <<std::endl;
        ++i;
        std::cout <<std::endl;
    }

    return true;
}
