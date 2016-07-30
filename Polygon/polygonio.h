#ifndef POLYGONIO_H
#define POLYGONIO_H

#include "polygonexpansion.h"
#include "polygonset.h"

class PolygonIO
{
public:
    PolygonIO();
    PolygonSet importPolygon(std::string file_path);
    void exportPolygon(PolygonSet set, std::string file_path);
};

#endif // POLYGONIO_H
