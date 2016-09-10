#ifndef ALGORITHMWRAPPER_H
#define ALGORITHMWRAPPER_H

#include "field.h"
#include "Utils/polygonconnector.h"
#include "Utils/fit.h"
#include "polygonviewer.h"

class AlgorithmWrapper
{
public:
    AlgorithmWrapper();

    virtual procon::Field run(procon::Field field);

    Fit fit1,fit2;

    int searchSameLength(procon::ExpandedPolygon polygon1 ,procon::ExpandedPolygon polygon2, std::vector<std::array<Fit,2>> &result);
    void findEnd(procon::ExpandedPolygon polygon1, procon::ExpandedPolygon polygon2,int &comp1,int &comp2, double length_error, double angle_error, int &Eva);

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
