#include "algorithmwrapper.h"
#include <math.h>

AlgorithmWrapper::AlgorithmWrapper()
{

}

procon::Field AlgorithmWrapper::run(procon::Field field)
{
    return field;
}

procon::ExpandedPolygon joinPolygon(ExpandedPolygon Polygon1,int Polygon1_start_pos,int Polygon1_end_pos, ExpandedPolygon Polygon2,int Polygon2_start_pos,int Polygon2_end_pos)
{
    // heikouidou
    polygon_t movePolygon;// Polygon2noidouato
    int point_differentx = Polygon2.getPolygon().outer()[Polygon2_start_pos].x - Polygon1.getPolygon().outer()[Polygon1_start_pos].x;
    int point_differenty = Polygon2.getPolygon().outer()[Polygon2_start_pos].y - Polygon1.getPolygon().outer()[Polygon1_start_pos].y;
    int size2 = Polygon2.getSize().outer();// Polygon2nokakusuu
    for (int i=0; i<size2; i++)// Polygon2nosubetenokakuwoidou
    {
        movePolygon.getPolygon().outer()[i].x = Polygon2.getPolygon().outer()[i].x - point_differentx;
        movePolygon.getPolygon().outer()[i].y = Polygon2.getPolygon().outer()[i].y - point_differenty;
    }

    // kaitenidou
    int x1 = movePolygon.getPolygon().outer()[Polygon2_end_pos].x - Polygon1.getPolygon().outer()[Polygon1_start_pos].x;
    int y1 = movePolygon.getPolygon().outer()[Polygon2_end_pos].y - Polygon1.getPolygon().outer()[Polygon1_start_pos].y;
    int x2 = Polygon1.getPolygon().outer()[Polygon1_end_pos].x - Polygon1.getPolygon().outer[Polygon1_start_pos].x;
    int y2 = Polygon1.getPolygon().outer()[Polygon1_end_pos].y - Polygon1.getPolygon().outer[Polygon1_start_pos].y;
    int cos = (x1*x2 + y1*y2)/(sqrt(x1*x1 + y1*y1)*sqrt(x2*x2 + y2*y2));// idoumaetoidougonokaitennkakudo
    int sin = sqrt(1 - cos*cos);
    for (int i=0; i<size2; i++)// Polygon2wokaitennidou
    {
        if (i != Polygon2_end_pos)
        {
            int move_x = movePolygon.getPolygon().outer()[i].x - Polygon1.getPolygon().outer()[Polygon1_start_pos].x;
            int move_y = movePolygon.getPolygon().outer()[i].y - Polygon1.getPolygon().outer()[Polygon1_start_pos].y;
            movePolygon.getPolygon().outer()[i].x = move_x * cos - move_y * sin + Polygon1.getPolygon().outer()[Polygon1_start_pos].x;
            movePolygon.getPolygon().outer()[i].y = move_x * sin + move_y * cos + Polygon1.getPOlygon().outer()[Polygon1_start_pos].y;
        }
    }
    for (int i=0; i<size1; i++)
    {
        movePolygon.getPolygon().outer()[size2+i].x = Polygon1.getPolygon().outer()[i].x;
        movePolygon.getPolygon().outer()[size2+i].y = Polygon1.getPolygon().outer()[i].y;
    }
    polygon_t newPolygon;
    int size1 = Polygon1.getSize().outer();

    return newPolygon;
}

procon::ExpandedPolygon
{

}
