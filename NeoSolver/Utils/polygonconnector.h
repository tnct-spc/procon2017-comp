#ifndef POLYGONCONNECTOR_H
#define POLYGONCONNECTOR_H

class Connect
{
public:
    //接するべき辺の若い方のpointのindex
    int frame_side_index = -1;
    int polygon_side_index = -1;

    //触れてるべき頂点のindex
    int frame_point_index = -1;
    int polygon_point_index = -1;

};

class PolygonConnector
{
public:
    PolygonConnector();
};

#endif // POLYGONCONNECTOR_H
