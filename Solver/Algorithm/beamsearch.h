#ifndef BEAMSEARCH_H
#define BEAMSEARCH_H

#include "algorithmwrapper.h"


class BeamSearch : public AlgorithmWrapper
{
private:
protected:
    //ビーム幅
    int beam_width;
    int cpu_num;
    int variety_width;
    void initialization();
    virtual void evaluateNextMove
    (std::vector<Evaluation> & evaluations,std::vector<procon::Field> const& field_vec);
    virtual std::vector<procon::Field> makeNextField
    (std::vector<Evaluation> const& evaluations,std::vector<procon::Field> const& field_vec);
    bool removeDuplicateField(std::vector<procon::Field> & field_vec);
    bool canPrune(procon::ExpandedPolygon const& next_frame ,double const& min_angle);
public:
    BeamSearch();
    void run(procon::Field field);

    bool alpha_is_none = true;
    bool beta_is_none = false;
    bool gamma_is_none = false;
    bool delta_is_none = true;
    bool epsilon_is_none = true;

    double alpha = 0;
    double beta = 0;
    double gamma = 4;
    double delta = 0;
    double epsilon = 0;
};


#endif // BEAMSEARCH_H
