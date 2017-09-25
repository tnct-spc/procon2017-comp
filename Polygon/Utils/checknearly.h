#ifndef CHECKNEARLY_H
#define CHECKNEARLY_H

#include "expandedpolygon.h"
#include "neoexpandedpolygon.h"

class CheckNearly
{
public:
    CheckNearly();
    static double check_nearly(procon::ExpandedPolygon expoly_d_in, procon::NeoExpandedPolygon expoly_i_in);
    static std::vector<procon::ExpandedPolygon> fit_nearly(std::vector<procon::ExpandedPolygon> expolys_d_in, std::vector<procon::NeoExpandedPolygon> expolys_i_in);
};

#endif // CHECKNEARLY_H
