#include "algorithmwrapper.h"
#include "utilities.h"
#include "Evaluation/searchsamelength.h"

#include <math.h>

#define PI 3.141592

procon::Field AlgorithmWrapper::run(procon::Field field)
{
    return field;
}

std::vector<Evaluation> AlgorithmWrapper::evaluateCombination(procon::ExpandedPolygon frame, procon::ExpandedPolygon piece)
{
    std::vector<Evaluation> evaluations;

    std::vector<std::array<Fit,2>> fits;
    std::vector<int> eva_point = SearchSameLength::evaluateMatching(frame, piece, fits);

    for(unsigned int fit_number=0; fit_number < fits.size(); ++fit_number){
        if(eva_point[fit_number] > 0){
            Evaluation eva;
            eva.frame_id = fits[fit_number].at(0).flame_inner_pos;
            eva.fits = fits[fit_number];
            eva.evaluation = eva_point[fit_number];
            evaluations.push_back(eva);
        }
    }

    return evaluations;
}

AlgorithmWrapper::AlgorithmWrapper()
{
}
