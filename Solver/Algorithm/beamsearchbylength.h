#ifndef BEAMSEARCHBYLENGTH_H
#define BEAMSEARCHBYLENGTH_H

#include "beamsearch.h"

class BeamSearchByLength : public BeamSearch
{
protected:
    void evaluateNextMove(std::vector<Evaluation> & evaluations,std::vector<procon::Field> const& field_vec);
public:
    BeamSearchByLength();
};

#endif // BEAMSEARCHBYLENGTH_H
