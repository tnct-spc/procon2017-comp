#ifndef POLYGONIO_H
#define POLYGONIO_H

#include "polygonexpansion.h"
#include "polygonset.h"

class PolygonIO
{
public:
    PolygonIO();
    static PolygonSet importPolygon(std::string file_path);
    static void exportPolygon(PolygonSet set, std::string file_path);
};

#endif // POLYGONIO_H
