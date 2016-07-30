#ifndef POLYGONSET_H
#define POLYGONSET_H

#include <vector>
#include "polygonexpansion.h"

class PolygonSet
{
public:
    PolygonSet();
    PolygonExpansion fieldFlame;
    std::vector<PolygonExpansion> fieldPiece;
};

#endif // POLYGONSET_H
