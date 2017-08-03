#include "testevaluation.h"
#include "neoexpandedpolygon.h"
#include "Evaluation/evaluation.h"
#include "Utils/polygonconnector.h"

TestEvaluation::TestEvaluation()
{

}

bool TestEvaluation::run()
{
    std::vector<procon::NeoExpandedPolygon> frames;
    procon::NeoExpandedPolygon nep;
    polygon_i boost_polygon;

    bg::exterior_ring ( boost_polygon ) = boost::assign::list_of < point_i >
            (2,4) (3,12) (5,13) (6,13) (7,11) (10,2) (7,2) (2,4);
    nep.resetPolygonForce ( boost_polygon );
    frames.push_back ( nep );

    bg::exterior_ring ( boost_polygon ) = boost::assign::list_of < point_i >
            (10,15) (15,15) (15,14) (17,14) (17,15) (20,15) (19,6) (10,15);
    nep.resetPolygonForce(boost_polygon);
    frames.push_back(nep);

    bg::exterior_ring ( boost_polygon ) = boost::assign::list_of < point_i >
            (12,3) (17,4) (19,2) (12,3);
    nep.resetPolygonForce ( boost_polygon );
    frames.push_back ( nep );


    std::vector<procon::NeoExpandedPolygon> polygons;
    bg::exterior_ring(boost_polygon) = boost::assign::list_of<point_i>
            (1,11)(2,15)(3,13)(3,12)(1,11);
    nep.resetPolygonForce(boost_polygon);
    polygons.push_back(nep);
    bg::exterior_ring(boost_polygon) = boost::assign::list_of<point_i>
            (3,5)(3,8)(10,8)(10,5)(3,5);
    nep.resetPolygonForce(boost_polygon);
    polygons.push_back(nep);
    bg::exterior_ring(boost_polygon) = boost::assign::list_of<point_i>
            (7,11)(7,17)(9,13)(9,12)(7,11);
    nep.resetPolygonForce(boost_polygon);
    polygons.push_back(nep);
    bg::exterior_ring(boost_polygon) = boost::assign::list_of<point_i>
            (13,12)(15,15)(17,12)(13,12);
    nep.resetPolygonForce(boost_polygon);
    polygons.push_back(nep);
    bg::exterior_ring(boost_polygon) = boost::assign::list_of<point_i>
            (10,9)(10,10)(18,9)(10,9);
    nep.resetPolygonForce(boost_polygon);
    polygons.push_back(nep);
    bg::exterior_ring(boost_polygon) = boost::assign::list_of<point_i>
            (13,4)(13,7)(15,7)(15,5)(16,5)(16,4)(13,4);
    nep.resetPolygonForce(boost_polygon);
    polygons.push_back(nep);
    bg::exterior_ring(boost_polygon) = boost::assign::list_of<point_i>
            (20,5)(20,8)(21,6)(22,8)(22,5)(20,5);
    nep.resetPolygonForce(boost_polygon);
    polygons.push_back(nep);

    std::vector<std::pair<int , Connect>> vector;

    Evaluation e;
    vector = e.evaluation(frames.at(0),polygons.at(6));

    int evaluation , field_side_index , polygon_side_index , field_point_index , polygon_point_index;
    for(std::pair<int , Connect> i : vector){
        evaluation = i.first;
        Connect connect = i.second;
        field_side_index = connect.frame_side_index;
        polygon_side_index = connect.polygon_side_index;
        field_point_index = connect.frame_point_index;
        polygon_point_index = connect.polygon_point_index;

        if(polygon_point_index != 2){
            std::cout << "evaluation = " << evaluation
                      << " , field_side_index = " << field_side_index
                      << " , polygon_side_index = " << polygon_side_index
                      << " , field_point_index = " << field_point_index
                      << " , polygon_point_index = " << polygon_point_index
                      << std::endl;
        }
    }

    return true;
}
