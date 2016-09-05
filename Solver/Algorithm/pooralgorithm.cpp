#include "pooralgorithm.h"

PoorAlgorithm::PoorAlgorithm()
{

}

procon::Field PoorAlgorithm::run(procon::Field field)
{
    /*
    polygon_t polygon;
    polygon.outer().push_back(point_t(0.002,0));
    polygon.outer().push_back(point_t(3.514,0.008));
    polygon.outer().push_back(point_t(3.506,4.013));
    polygon.outer().push_back(point_t(2.507,2.006));
    polygon.outer().push_back(point_t(2.003,2.004));
    polygon.outer().push_back(point_t(0.508,5.009));
    polygon.outer().push_back(point_t(0.011,5.017));
    polygon.outer().push_back(point_t(0.002,0));
    procon::ExpandedPolygon ex_polygon1;
    ex_polygon1.setPolygon(polygon);
    ex_polygon1.updatePolygon();

    polygon_t polygon2;
    polygon2.outer().push_back(point_t(1.506,0.002));
    polygon2.outer().push_back(point_t(2.009,0));
    polygon2.outer().push_back(point_t(3.503,2.996));
    polygon2.outer().push_back(point_t(3.501,4.015));
    polygon2.outer().push_back(point_t(0.009,4.002));
    polygon2.outer().push_back(point_t(0.014,3.008));
    polygon2.outer().push_back(point_t(1.506,0.002));
    procon::ExpandedPolygon ex_polygon2;
    ex_polygon2.setPolygon(polygon2);
    ex_polygon2.updatePolygon();

    std::vector<std::array<Fit,2>> result;
    const int status = searchSameLength(ex_polygon1,ex_polygon2,result);
    const int size = result.size();

    if (status != 0 ){
        std::cout << "Evaluation for Piece1(" << ex_polygon1.getSize() << ") and Piece2(" << ex_polygon2.getSize() << ") ." << std::endl;
        std::cout << boost::geometry::dsv(ex_polygon1.getPolygon()) << std::endl;
        std::cout << boost::geometry::dsv(ex_polygon2.getPolygon()) << std::endl;
        std::cout << "status is " << status << "." << std::endl;
        std::cout << "result size is " << size << "." << std::endl;
    }
    std::cout << "Piece Evaluation Completed..." << std::endl;
    */

    field.setFlame(field.getElementaryFlame());
    field.setPiece(field.getElementaryPieces().at(0),15.0,15.0);
    const int maxpiece = static_cast<int>(field.getElementaryPieces().size());
    for (int i = 0 ; i < maxpiece ; i++) {
        for (int j = i + 1 ; j< maxpiece ; j++) {
            std::vector<std::array<Fit,2>> result;
            auto piece1 = field.getElementaryPieces().at(i);
            auto piece2 = field.getElementaryPieces().at(j);
            piece1.updatePolygon();
            piece2.updatePolygon();
            const int status = searchSameLength(piece1,piece2,result);
            const int size = result.size();
            if (status != 0 ){
                std::cout << "Evaluation for Piece" << i << "(" << piece1.getSize() << ") and Piece" << j <<"(" << piece2.getSize() << ") ." << std::endl;
                std::cout << boost::geometry::dsv(piece1.getPolygon()) << std::endl;
                std::cout << boost::geometry::dsv(piece2.getPolygon()) << std::endl;
                std::cout << "status is " << status << "." << std::endl;
                std::cout << "result size is " << size << "." << std::endl;
            }
        }
    }
    std::cout << "Piece Evaluation Completed..." << std::endl;
    return field;

}

