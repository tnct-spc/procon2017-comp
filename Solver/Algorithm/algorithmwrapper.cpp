#include "algorithmwrapper.h"
#include <math.h>

#define PI 3.141592

AlgorithmWrapper::AlgorithmWrapper()
{

}

procon::Field AlgorithmWrapper::run(procon::Field field)
{
    return field;
}



procon::ExpandedPolygon joinPolygon(procon::ExpandedPolygon Polygon1,int Polygon1_start_pos,int Polygon1_end_pos, procon::ExpandedPolygon Polygon2,int Polygon2_start_pos,int Polygon2_end_pos)
{
    double x2 = Polygon2.getPolygon().outer()[Polygon2_end_pos].x() - Polygon2.getPolygon().outer()[Polygon2_start_pos].x();
    double y2 = Polygon2.getPolygon().outer()[Polygon2_end_pos].y() - Polygon2.getPolygon().outer()[Polygon2_start_pos].y();
    double degree2 = atan2(y2, x2) * 180 / PI;

    double x1 = Polygon1.getPolygon().outer()[Polygon1_end_pos].x() - Polygon1.getPolygon().outer()[Polygon1_start_pos].x();
    double y1 = Polygon1.getPolygon().outer()[Polygon1_end_pos].y() - Polygon1.getPolygon().outer()[Polygon1_start_pos].y();
    double degree1 = atan2(y1, x1) * 180 / PI;

    double degree = degree1 - degree2;
    Polygon2.rotatePolygon(degree);

    double move_x = Polygon2.getPolygon().outer()[Polygon2_start_pos].x() - Polygon1.getPolygon().outer()[Polygon1_start_pos].x();
    double move_y = Polygon2.getPolygon().outer()[Polygon2_start_pos].y() - Polygon1.getPolygon().outer()[Polygon1_start_pos].y();

    double sumx = 0, sumy = 0;
    int size2 = Polygon2.getSize();

    for (int i=0; i<size2; i++)
    {
        sumx = sumx + Polygon2.getPolygon().outer()[i].x() - move_x;
        sumy = sumy + Polygon2.getPolygon().outer()[i].y() - move_y;
    }
    double balance_x = sumx / size2;
    double balance_y = sumy / size2;
    Polygon2.setPolygonPosition(balance_x, balance_y);

    polygon_t new_pieces;
    int count=0;
    int Type=1;
    int size1 = Polygon1.getSize();
    while (count < size1)
    {
        double x,y;
        if (Type == 1)
        {
            x = Polygon1.getPolygon().outer()[count].x();
            y = Polygon1.getPolygon().outer()[count].y();
            if (count == Polygon1_start_pos)
            {
                Type = 2;
                count = Polygon2_start_pos;
            }
            else if (count == Polygon1_end_pos)
            {
                Type = 2;
                count = Polygon2_end_pos;
            }
        }
        else if (Type == 2)
        {
            x = Polygon2.getPolygon().outer()[count].x();
            y = Polygon2.getPolygon().outer()[count].y();
            if (count == Polygon2_start_pos)
            {
                Type = 1;
                count = Polygon1_start_pos;
            }
            else if (count == Polygon2_end_pos)
            {
                Type = 1;
                count = Polygon1_end_pos;
            }
        }
        new_pieces.outer().push_back(point_t(x,y));
        count++;
    }
    procon::ExpandedPolygon new_polygon;
    new_polygon.setPolygon(new_pieces);
    return new_polygon;
}
