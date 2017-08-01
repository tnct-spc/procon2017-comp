#include "testevaluation.h"
#include "neoexpandedpolygon.h"
#include "Evaluation/evaluation.h"

TestEvaluation::TestEvaluation()
{

}


bool TestEvaluation::run()
{
    std::vector<procon::NeoExpandedPolygon> frames;
    procon::NeoExpandedPolygon frame;
    frame.resetPolygonForce({{{2,4},{3,12},{5,13},{6,13},{7,11},{10,2},{2,4}}});
    frames.push_back(frame);
    frame.resetPolygonForce({{{10,15},{15,15},{15,14},{17,14},{17,15},{20,15},{19,6},{10,15}}});
    frames.push_back(frame);
    frame.resetPolygonForce({{{12,3},{17,4},{19,2},{12,3}}});
    frames.push_back(frame);


    std::vector<procon::NeoExpandedPolygon> polygons;
    procon::NeoExpandedPolygon polygon;
    polygon.resetPolygonForce({{{1,11},{2,15},{3,13},{3,12},{1,11}}});
    polygons.push_back(polygon);
    polygon.resetPolygonForce({{{3,5},{3,8},{10,8},{10,5},{3,5}}});
    polygons.push_back(polygon);
    polygon.resetPolygonForce({{{7,11},{7,17},{9,13},{9,12},{7,11}}});
    polygons.push_back(polygon);
    polygon.resetPolygonForce({{{13,12},{15,15},{17,12},{13,12}}});
    polygons.push_back(polygon);
    polygon.resetPolygonForce({{{10,9},{10,10},{18,9},{10,9}}});
    polygons.push_back(polygon);
    polygon.resetPolygonForce({{{13,4},{13,7},{15,7},{15,5},{16,5},{16,4},{13,4}}});
    polygons.push_back(polygon);
    polygon.resetPolygonForce({{{20,5},{20,8},{21,6},{22,8},{22,5},{20,5}}});
    polygons.push_back(polygon);

    Evaluation evaluation;
    evaluation.evaluation(frames,polygons.at(0));

    std::cout << boost::geometry::dsv(frames.at(1).getPolygon().outer().at(0)) << std::endl;
    std::cout << boost::geometry::dsv(frames.at(1).getPolygon().outer().at(1)) << std::endl;
    std::cout << frames.at(1).getSideLength().at(0) << std::endl;


    return true;
}
