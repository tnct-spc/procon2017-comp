#include "algorithmwrapper.h"
#include <math.h>

#define PI 3.141592

procon::Field AlgorithmWrapper::run(procon::Field field)
{
    return field;
}

procon::ExpandedPolygon joinPolygon(procon::ExpandedPolygon Polygon1,int Polygon1_start_pos,int Polygon1_end_pos, procon::ExpandedPolygon Polygon2,int Polygon2_start_pos,int Polygon2_end_pos)
{
    int size1 = Polygon1.getSize() + 1;
    int size2 = Polygon2.getSize() + 1;
    double x1 = Polygon1.getPolygon().outer()[(Polygon1_start_pos + 1) % size1].x() - Polygon1.getPolygon().outer()[Polygon1_start_pos].x();
    double y1 = Polygon1.getPolygon().outer()[(Polygon1_start_pos + 1) % size1].y() - Polygon1.getPolygon().outer()[Polygon1_start_pos].y();
    double x2 = Polygon2.getPolygon().outer()[(Polygon2_start_pos + size2 - 1) % size2].x() - Polygon2.getPolygon().outer()[Polygon2_start_pos].x();
    double y2 = Polygon2.getPolygon().outer()[(Polygon2_start_pos + size2 - 1) % size2].y() - Polygon2.getPolygon().outer()[Polygon2_start_pos].y();
    double degree2 = atan2(y2, x2);
    double degree1 = atan2(y1, x1);
    double degree = (degree1 - degree2) * 180 / PI;

    double rad = degree * PI / 180;
    polygon_t turn_polygon;
    for (int i=0; i<size2; i++)
    {
        double x = Polygon2.getPolygon().outer()[i].x() - Polygon2.getPolygon().outer()[0].x();
        double y = Polygon2.getPolygon().outer()[i].y() - Polygon2.getPolygon().outer()[0].y();
        double move_x = (x * cos(rad)) - (y * sin(rad));
        double move_y = (x * sin(rad)) + (y * cos(rad));
        double turn_x = move_x + Polygon2.getPolygon().outer()[0].x();
        double turn_y = move_y + Polygon2.getPolygon().outer()[0].y();
        turn_polygon.outer().push_back(point_t(turn_x, turn_y));
    }
    Polygon2.setPolygon(turn_polygon);

    int Join_point1 = (Polygon1_start_pos + 1) % size1;
    int Join_point2 = (Polygon2_start_pos + size2 - 1) % size2;
    double move_x = Polygon1.getPolygon().outer()[Join_point1].x() - Polygon2.getPolygon().outer()[Join_point2].x();
    double move_y = Polygon1.getPolygon().outer()[Join_point1].y() - Polygon2.getPolygon().outer()[Join_point2].y();

    Polygon2.translatePolygon(move_x, move_y);

    polygon_t new_pieces;
    int count = Polygon1_end_pos + 1;
    int Type = 1;

    double x1_st = Polygon1.getPolygon().outer()[(Polygon1_start_pos + 1) % size1].x() - Polygon1.getPolygon().outer()[Polygon1_start_pos].x();
    double y1_st = Polygon1.getPolygon().outer()[(Polygon1_start_pos + 1) % size1].y() - Polygon1.getPolygon().outer()[Polygon1_start_pos].y();
    double x2_st = Polygon2.getPolygon().outer()[(Polygon2_start_pos + size2 - 1) % size1].x() - Polygon2.getPolygon().outer()[Polygon2_start_pos].x();
    double y2_st = Polygon2.getPolygon().outer()[(Polygon2_start_pos + size2 - 1) % size1].y() - Polygon2.getPolygon().outer()[Polygon2_start_pos].y();
    double x1_end = Polygon1.getPolygon().outer()[(Polygon1_end_pos + size1 - 1) % size1].x() - Polygon1.getPolygon().outer()[Polygon1_end_pos].x();
    double y1_end = Polygon1.getPolygon().outer()[(Polygon1_end_pos + size1 - 1) % size1].y() - Polygon1.getPolygon().outer()[Polygon1_end_pos].y();
    double x2_end = Polygon2.getPolygon().outer()[(Polygon2_end_pos + 1) % size1].x() - Polygon2.getPolygon().outer()[Polygon2_end_pos].x();
    double y2_end = Polygon2.getPolygon().outer()[(Polygon2_end_pos + 1) % size1].y() - Polygon2.getPolygon().outer()[Polygon2_end_pos].y();

    do
    {
        double x,y;
        if (Type == 1)
        {
            x = Polygon1.getPolygon().outer()[count%size1].x();
            y = Polygon1.getPolygon().outer()[count%size1].y();
            if (count % size1 == Polygon1_start_pos)
            {
                Type = 2;
                if ((x1_st == x2_st) && (y1_st == y2_st))
                {
                    count = Polygon2_start_pos;
                }
                else
                {
                    count = Polygon2_start_pos - 1;
                }
            }
        }
        else if (Type == 2)
        {
            x = Polygon2.getPolygon().outer()[count%size2].x();
            y = Polygon2.getPolygon().outer()[count%size2].y();
            if (count % size2 == Polygon2_end_pos)
            {
                Type = 1;
                if ((x1_end == x2_end) && (y1_end == y2_end))
                {
                    count = Polygon1_end_pos;
                }
                else
                {
                    count = Polygon1_end_pos - 1;
                }
            }
        }
        new_pieces.outer().push_back(point_t(x,y));
        count++;
    } while ((Type != 1) || (count != (Polygon1_end_pos + 1)));

    procon::ExpandedPolygon new_polygon;
    new_polygon.setPolygon(new_pieces);
    return new_polygon;
}

AlgorithmWrapper::AlgorithmWrapper()
{
    polygon_t sample11;
    sample11.outer().push_back(point_t(0,0));
    sample11.outer().push_back(point_t(0,2));
    sample11.outer().push_back(point_t(1,2));
    sample11.outer().push_back(point_t(1,1));
    sample11.outer().push_back(point_t(2,1));
    sample11.outer().push_back(point_t(2,0));

    polygon_t sample12;
    sample12.outer().push_back(point_t(0,0));
    sample12.outer().push_back(point_t(0,1));
    sample12.outer().push_back(point_t(2,2));
    sample12.outer().push_back(point_t(2,0));

    procon::ExpandedPolygon polygon1(0);
    procon::ExpandedPolygon polygon2(1);
    polygon1.setPolygon(sample11);
    polygon2.setPolygon(sample12);

    procon::ExpandedPolygon Joined = joinPolygon(polygon1,2,4,polygon2,1,3);
}
