#ifndef CHECKNEARLY_H
#define CHECKNEARLY_H

#include "expandedpolygon.h"
#include "neoexpandedpolygon.h"

class CheckNearly
{
public:
    CheckNearly();
    static std::vector<procon::ExpandedPolygon> fit_nearly(const std::vector<procon::ExpandedPolygon> &expolys_d_in, const std::vector<procon::NeoExpandedPolygon> &expolys_i_in);
};

#endif // CHECKNEARLY_H
