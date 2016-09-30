#ifndef ALGORITHMWRAPPER_H
#define ALGORITHMWRAPPER_H

#include "field.h"
#include "expandedpolygon.h"
#include "Utils/evaluation.h"
#include "fit.h"

class AlgorithmWrapper
{
public:
    static constexpr double length_error = 0.05; // 単位CM
    static constexpr double angle_error = 0.017; //単位rad 0.017rad=1°

    static constexpr double ideal_min = 0.1;
    static constexpr double ideal_max = 1.5;

    std::vector<double> angle_frequency;

    AlgorithmWrapper();
    virtual ~AlgorithmWrapper() = default;

    virtual procon::Field run(procon::Field field);
    void calcAngleFrequency(procon::Field field);

    Fit fit1,fit2;
    std::vector<Evaluation> evaluateCombinationByAngle(procon::ExpandedPolygon const& frame, procon::ExpandedPolygon const& piece);
    std::vector<Evaluation> evaluateCombinationByLength(procon::ExpandedPolygon const& frame, procon::ExpandedPolygon const& piece);

    typedef struct PieceAssesment{
        //評価値
        int EvaluationValue;
        //評価したpolygon
        procon::ExpandedPolygon Polygon;

        bool operator<(const PieceAssesment& Pieceassesment) const {
            return EvaluationValue < Pieceassesment.EvaluationValue;
        }

    } PieceAssesment;


};

#endif // ALGORITHMWRAPPER_H
