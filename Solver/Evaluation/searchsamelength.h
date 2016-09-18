#ifndef SEARCHSAMELENGTH_H
#define SEARCHSAMELENGTH_H

#include "expandedpolygon.h"
#include "Algorithm/algorithmwrapper.h"

class SearchSameLength
{
private:
    SearchSameLength();
public:
    static std::vector<int> evaluateMatching(procon::ExpandedPolygon polygon1, procon::ExpandedPolygon polygon2, std::vector<std::array<Fit,2>> &result);
};

#endif // SEARCHSAMELENGTH_H
