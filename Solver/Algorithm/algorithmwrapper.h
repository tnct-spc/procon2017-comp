#ifndef ALGORITHMWRAPPER_H
#define ALGORITHMWRAPPER_H

#include "field.h"
#include "expandedpolygon.h"
#include "Utils/evaluation.h"
#include "answerdock.h"
#include "fit.h"

class AlgorithmWrapper : public QObject
{
    Q_OBJECT

public:
    static constexpr double length_error = 0.10; // 単位CM
    static constexpr double angle_error = 0.034; //単位rad 0.017rad=1°

    std::vector<double> angle_frequency;
    std::vector<std::vector<double>> angle_frequency_kai;
    std::vector<double> length_frequency;

    //angle-Frequency
    static constexpr int angle_resolution = 5;
    //liner
    static constexpr double angle_ideal_min = 0.1;
    static constexpr double angle_ideal_max = 1.5;
    //exponential
    static constexpr double angle_alpha = 1;
    static constexpr double angle_beta = 0;
    static constexpr double angle_base = 2;

    //length-Frequency
    static constexpr double length_resolution = 1;
    static constexpr double length_alpha = 1;
    static constexpr double length_beta = 0;
    static constexpr double length_base = 2;

    //History
    double norm = 0;
    AlgorithmWrapper();
    virtual ~AlgorithmWrapper() = default;

    std::shared_ptr<AnswerDock> DOCK;

    void init();
    virtual void run(procon::Field field);
    void calcAngleFrequency(procon::Field field);
    void calcLengthFrequency(procon::Field field);
    void submitAnswer(procon::Field field);

    Fit fit1,fit2;
    std::vector<Evaluation> evaluateCombinationByAngle(procon::ExpandedPolygon const& frame, procon::ExpandedPolygon const& piece);
    std::vector<Evaluation> evaluateCombinationByLength(procon::ExpandedPolygon const& frame, procon::ExpandedPolygon const& piece);

    double evaluateUniqueAngle(Evaluation const& evaluation,std::vector<procon::Field> const& field_vec);
    double evaluateUniqueLength(Evaluation const& evaluation,std::vector<procon::Field> const& field_vec);
    double evaluateFrame(Evaluation const& evaluation,std::vector<procon::Field> const& field_vec);
    void evaluateHistoryInit(std::vector<procon::Field> const& field_vec);
    double evaluateHistory(Evaluation const& evaluation,std::vector<procon::Field> const& field_vec);
    typedef struct PieceAssesment{
        //評価値
        int EvaluationValue;
        //評価したpolygon
        procon::ExpandedPolygon Polygon;

        bool operator<(const PieceAssesment& Pieceassesment) const {
            return EvaluationValue < Pieceassesment.EvaluationValue;
        }

    } PieceAssesment;
signals:
    void throwAnswer(procon::Field field);
};

#endif // ALGORITHMWRAPPER_H
