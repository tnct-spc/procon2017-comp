#ifndef STEPSEARCH_H
#define STEPSEARCH_H

#include "Algorithm/algorithmwrapper.h"
#include "field.h"
#include "Evaluation/searchsamelength.h"
#include "Utils/evaluation.h"
#include <QEventLoop>

class StepSearch : public AlgorithmWrapper
{
private:
    //ビーム幅
    int beam_width = 100;

    void evaluateNextMove(std::vector<Evaluation> & evaluations,std::vector<procon::Field> const& field_vec);
    std::vector<procon::Field> makeNextField(std::vector<Evaluation> const& evaluations,std::vector<procon::Field> const& field_vec);
    bool canPrune(procon::ExpandedPolygon const& next_frame ,double const& min_angle);

public:
    StepSearch();
    void run(procon::Field field);
    std::vector<procon::Field> field_vec;
    QEventLoop loop;
private slots:
    void restartWithField(procon::Field field);
};

#endif // STEPSEARCH_H
