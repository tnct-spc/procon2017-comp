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
    const double length_error = 0.05; // 単位CM
    const double angle_error = 0.017; //単位rad 0.017rad=1°
public:
    AlgorithmWrapper();

    virtual procon::Field run(procon::Field field);

    Fit fit1,fit2;

    std::vector<int> searchSameLength(procon::ExpandedPolygon polygon1 ,procon::ExpandedPolygon polygon2, std::vector<std::array<Fit,2>> &result);
    std::vector<Evaluation> evaluateCombination(procon::ExpandedPolygon frame, procon::ExpandedPolygon piece);
    Fit::DotORLine findEnd(procon::ExpandedPolygon polygon1, procon::ExpandedPolygon polygon2,int &comp1,int &comp2,int &Eva);

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
