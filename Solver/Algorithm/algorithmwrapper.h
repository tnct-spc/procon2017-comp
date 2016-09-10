#ifndef ALGORITHMWRAPPER_H
#define ALGORITHMWRAPPER_H

#include "field.h"

class AlgorithmWrapper
{
public:
    enum DotORLine { Dot , Line };

    typedef struct FIT{
        DotORLine start_dot_or_line = Dot;
        int start_id = -1;
        DotORLine end_dot_or_line = Dot;
        int end_id = -1;
    } Fit;

    AlgorithmWrapper();

    procon::ExpandedPolygon newPolygonDate(procon::ExpandedPolygon polygon);
    procon::ExpandedPolygon joinPolygon(procon::ExpandedPolygon Polygon1,int Polygon1_start_pos,int Polygon1_end_pos, procon::ExpandedPolygon Polygon2,int Polygon2_start_pos,int Polygon2_end_pos);

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
