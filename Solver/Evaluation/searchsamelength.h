#ifndef SEARCHSAMELENGTH_H
#define SEARCHSAMELENGTH_H

#include "expandedpolygon.h"
#include "fit.h"

class SearchSameLength
{
private:
    SearchSameLength();
public:
    static std::vector<int> evaluateMatching(const procon::ExpandedPolygon& polygon1, const procon::ExpandedPolygon& polygon2, std::vector<std::array<Fit,2>> &result);
};

#endif // SEARCHSAMELENGTH_H
