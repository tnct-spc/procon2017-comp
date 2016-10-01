#include "algorithmwrapper.h"

#include "utilities.h"
#include "Utils/polygonconnector.h"
#include "polygonviewer.h"
#include "utilities.h"
#include "Evaluation/searchsamelength.h"
#include <QTimer>

void AlgorithmWrapper::init()
{
    DOCK = std::make_shared<AnswerDock>();
    DOCK->showMaximized();
}

void AlgorithmWrapper::submitAnswer(procon::Field field)
{
    emit throwAnswer(field);
    // Wait 1msec
    QEventLoop loop;
    QTimer::singleShot(1, &loop, SLOT(quit()));
    loop.exec();
}

procon::Field AlgorithmWrapper::run(procon::Field field)
{
    return field;
}
std::vector<Evaluation> AlgorithmWrapper::evaluateCombinationByAngle(procon::ExpandedPolygon const& frame, procon::ExpandedPolygon const& piece)
{
    double frame_first = 0;
    double piece_first = 0;
    std::vector<Evaluation> evaluations;

    for (unsigned int k = 0; k < frame.getPolygon().inners().size(); ++k){
        const int inner_size = static_cast<int>(frame.getPolygon().inners().at(k).size() - 1);
        for (int i = 0; i < inner_size; ++i) {
            frame_first = frame.getInnersSideAngle().at(k).at(i);
            for (int j = 0; j < piece.getSize(); ++j) {
                piece_first = piece.getSideAngle()[j];

                if(Utilities::nearlyEqual(frame_first,piece_first,angle_error)){
                    std::array<Fit,2> fits;
                    fits.at(0).start_id = i;
                    fits.at(0).end_id = i;
                    fits.at(0).frame_inner_pos = k;
                    fits.at(0).start_dot_or_line = Fit::Dot;
                    fits.at(0).end_dot_or_line = Fit::Dot;
                    fits.at(1).start_id = j;
                    fits.at(1).end_id = j;
                    fits.at(1).start_dot_or_line = Fit::Dot;
                    fits.at(1).end_dot_or_line = Fit::Dot;
                    Evaluation eva;
                    eva.frame_id = k;
                    eva.fits = fits;
                    eva.evaluation = bg::area(piece.getPolygon());
                    evaluations.push_back(eva);
                }
            }
        }
    }
    return evaluations;
}


std::vector<Evaluation> AlgorithmWrapper::evaluateCombinationByLength(procon::ExpandedPolygon const& frame, procon::ExpandedPolygon const& piece)
{
    std::vector<Evaluation> evaluations;

    std::vector<std::array<Fit,2>> fits;
    std::vector<int> eva_point = SearchSameLength::evaluateMatching(frame, piece, fits);

    for(unsigned int fit_number=0; fit_number < fits.size(); ++fit_number){
        if(eva_point[fit_number] > 0){
            Evaluation eva;
            eva.frame_id = fits[fit_number].at(0).frame_inner_pos;
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
