#include "testsearchsamelength.h"

#include "polygonviewer.h"
#include "Utils/polygonconnector.h"
#include "Evaluation/searchsamelength.h"
#include "utilities.h"

testSearchSameLength::testSearchSameLength()
{
}
bool testSearchSameLength::run(){
    //1
    {
        procon::ExpandedPolygon polygon1(0);
        procon::ExpandedPolygon polygon2(1);
        procon::ExpandedPolygon result;

        polygon_t sample11;
        sample11.outer().push_back(point_t(0,20));
        sample11.outer().push_back(point_t(5,10));
        sample11.outer().push_back(point_t(0,0));
        sample11.outer().push_back(point_t(-5,30));
        sample11.outer().push_back(point_t(10,30));
        sample11.outer().push_back(point_t(0,20));

        polygon_t sample12;
        sample12.outer().push_back(point_t(-2.5,-5));
        sample12.outer().push_back(point_t(2.5,-15));
        sample12.outer().push_back(point_t(-2.5,-20));
        sample12.outer().push_back(point_t(-10,0));
        sample12.outer().push_back(point_t(0,0));
        sample12.outer().push_back(point_t(-2.5,-5));

        polygon1.resetPolygonForce(sample11);
        polygon2.resetPolygonForce(sample12);
        std::vector<std::array<Fit,2>> fits;
        //SearchSameLength::evaluateMatching(polygon1, polygon2, fits);

        //int cnt=0;
        for(auto join_data : fits){
              bool conf = PolygonConnector::joinPolygon(polygon1, polygon2, result, join_data);
              if(conf==false){
                  debugprint("return false");
                  return false;
              }
            //PolygonViewer::getInstance().pushPolygon(result, std::to_string(++cnt),50);
        }
        //PolygonViewer::getInstance().pushPolygon(polygon1, TO_STRING(polygon1),50);
        //PolygonViewer::getInstance().pushPolygon(polygon2, TO_STRING(polygon2),50);

    }
    //2
    {
        procon::ExpandedPolygon polygon1(0);
        procon::ExpandedPolygon polygon2(1);
        procon::ExpandedPolygon result;

        polygon_t sample11;
        sample11.outer().push_back(point_t(-5,-5));
        sample11.outer().push_back(point_t(-5,10));
        sample11.outer().push_back(point_t( 5,10));
        sample11.outer().push_back(point_t( 5,0));
        sample11.outer().push_back(point_t(10,0));
        sample11.outer().push_back(point_t(10,-5));
        sample11.outer().push_back(point_t(0,-10));
        sample11.outer().push_back(point_t(-5,-5));

        polygon_t sample12;
        sample12.outer().push_back(point_t(5,5));
        sample12.outer().push_back(point_t(5,-5));
        sample12.outer().push_back(point_t(-5,-5));
        sample12.outer().push_back(point_t(-5,0));
        sample12.outer().push_back(point_t(0,0));
        sample12.outer().push_back(point_t(0,5));
        sample12.outer().push_back(point_t(5,5));

        polygon1.resetPolygonForce(sample11);
        polygon2.resetPolygonForce(sample12);

        std::vector<std::array<Fit,2>> fits;
        //SearchSameLength::evaluateMatching(polygon1, polygon2, fits);
        int cnt=0;
        for(auto join_data : fits){
            ++cnt;
            bool conf = PolygonConnector::joinPolygon(polygon1, polygon2, result, join_data);
            if(conf==false){
                debugprint(std::to_string(cnt)+"is fail! it's can't connect.");
                //return false;
            }else{
                //PolygonViewer::getInstance().pushPolygon(result, std::to_string(cnt),50);
            }
        }
        //PolygonViewer::getInstance().pushPolygon(polygon1, TO_STRING(polygon1),50);
        //PolygonViewer::getInstance().pushPolygon(polygon2, TO_STRING(polygon2),50);
        /*
        std::array<Fit,2> join_data;
        {
            Fit fit1,fit2;
            fit1.start_dot_or_line = Fit::Dot;
            fit1.start_id = 3;
            fit1.end_dot_or_line = Fit::Line;
            fit1.end_id = 4;
            fit2.start_dot_or_line = Fit::Dot;
            fit2.start_id = 5;
            fit2.end_dot_or_line = Fit::Line;
            fit2.end_id = 3;

            join_data.at(0) = fit1;
            join_data.at(1) = fit2;
        }

        result = PolygonConnector::joinPolygon(polygon1, polygon2, join_data);
        */
        //PolygonViewer::getInstance().pushPolygon(result, TO_STRING(result),50);

    }
    //3
    {
        procon::ExpandedPolygon polygon1(0);
        procon::ExpandedPolygon polygon2(1);
        procon::ExpandedPolygon result;

        polygon_t sample11;
        sample11.outer().push_back(point_t(0,0));
        sample11.outer().push_back(point_t(5,0));
        sample11.outer().push_back(point_t(5,5));
        sample11.outer().push_back(point_t(0,5));
        sample11.outer().push_back(point_t(0,0));
        sample11.inners().push_back(polygon_t::ring_type());
        sample11.inners().at(0).push_back(point_t(2,1));
        sample11.inners().at(0).push_back(point_t(1,2));
        sample11.inners().at(0).push_back(point_t(1,1));
        sample11.inners().at(0).push_back(point_t(2,1));
        sample11.inners().push_back(polygon_t::ring_type());
        sample11.inners().at(1).push_back(point_t(4,1));
        sample11.inners().at(1).push_back(point_t(4,3));
        sample11.inners().at(1).push_back(point_t(2,3));
        sample11.inners().at(1).push_back(point_t(2,2));
        sample11.inners().at(1).push_back(point_t(3,1));
        sample11.inners().at(1).push_back(point_t(4,1));

        polygon_t sample12;
        sample12.outer().push_back(point_t(0,-2));
        sample12.outer().push_back(point_t(-0.5,-2));
        sample12.outer().push_back(point_t(-1.5,-1));
        sample12.outer().push_back(point_t(-1.5,0));
        sample12.outer().push_back(point_t(-0.5,0));
        sample12.outer().push_back(point_t(-0.5,-0.5));
        sample12.outer().push_back(point_t(-1,-0.5));
        sample12.outer().push_back(point_t(0,-1.5));
        sample12.outer().push_back(point_t(0,-2));

        polygon1.resetPolygonForce(sample11);
        polygon2.resetPolygonForce(sample12);

        std::vector<std::array<Fit,2>> fits;
        SearchSameLength::evaluateMatching(polygon1, polygon2, fits);
        int cnt=0;
        for(auto join_data : fits){
            ++cnt;
            bool conf = PolygonConnector::joinPolygon(polygon1, polygon2, result, join_data);
            if(conf==false){
                debugprint(std::to_string(cnt)+"is fail! it's can't connect.");
                //return false;
            }else{
                PolygonViewer::getInstance().pushPolygon(result, std::to_string(cnt));
            }
        }
        PolygonViewer::getInstance().pushPolygon(polygon1, TO_STRING(polygon1));
        PolygonViewer::getInstance().pushPolygon(polygon2, TO_STRING(polygon2));

    }
    return true;
}
