#ifndef ALGORITHMWRAPPER_H
#define ALGORITHMWRAPPER_H

#include "field.h"
#include "utilities.h"
#include "Utils/polygonconnector.h"
#include "Utils/fit.h"
#include "Utils/evaluation.h"
#include "polygonviewer.h"

class AlgorithmWrapper
{
public:
    static constexpr double length_error = 0.05; // 単位CM
    static constexpr double angle_error = 0.017; //単位rad 0.017rad=1°

    AlgorithmWrapper();
    virtual ~AlgorithmWrapper(){}

    virtual procon::Field run(procon::Field field);

    Fit fit1,fit2;

    std::vector<Evaluation> evaluateCombination(procon::ExpandedPolygon frame, procon::ExpandedPolygon piece);

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
