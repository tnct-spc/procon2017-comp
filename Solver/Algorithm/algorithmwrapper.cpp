#include "algorithmwrapper.h"
#include "utilities.h"

#include <math.h>

#define PI 3.141592

procon::Field AlgorithmWrapper::run(procon::Field field)
{
    return field;
}

std::vector<int> AlgorithmWrapper::searchSameLength(procon::ExpandedPolygon polygon1, procon::ExpandedPolygon polygon2, std::vector<std::array<Fit,2>> &result)
{
    /*許容誤差指定*/
    double comped_first = 0;
    double comping_first = 0;
    int Eva = 0;
    //int maxEva = 0;
    std::vector<int> evaluation;
    std::array<Fit,2> fits;

    for (int i = 0; i < polygon1.getSize(); ++i) { //角度のみ探索
        comped_first = polygon1.getSideAngle()[i];
        for (int j = 0; j < polygon2.getSize(); ++j) {
            comping_first = polygon2.getSideAngle()[j];
            //if (procon::nearlyEqual(comped_first,comping_first,angle_error)) {
            if((M_PI * 2) - angle_error * 2 < (comping_first + comped_first) && (comping_first + comped_first) < (M_PI * 2) + angle_error * 2) {
                Eva++;

                int start_polygon1 = i;
                int start_polygon2 = j;
                Fit::DotORLine dot_or_line = AlgorithmWrapper::findEnd(polygon2, polygon1, start_polygon2, start_polygon1, Eva);
                fits.at(0).end_dot_or_line = dot_or_line;
                fits.at(0).end_id = start_polygon1;
                fits.at(1).end_dot_or_line = dot_or_line;
                fits.at(1).end_id = start_polygon2;

                //重複していたら追加しない
                bool isDuplicate = false;
                for(auto accept_fits : result){
                    if(fits[0].end_dot_or_line == accept_fits[0].end_dot_or_line &&
                       fits[0].end_id == accept_fits[0].end_id &&
                       fits[1].end_id == accept_fits[1].end_id &&
                       fits[1].end_dot_or_line == accept_fits[1].end_dot_or_line){
                        isDuplicate = true;
                        break;
                    }
                }

                if(!isDuplicate){
                    start_polygon1 = i;
                    start_polygon2 = j;
                    dot_or_line = AlgorithmWrapper::findEnd(polygon1, polygon2, start_polygon1, start_polygon2, Eva);
                    fits.at(0).start_dot_or_line = dot_or_line;
                    fits.at(0).start_id=start_polygon1;
                    fits.at(1).start_dot_or_line = dot_or_line;
                    fits.at(1).start_id=start_polygon2;
                    evaluation.push_back(Eva);
                    result.push_back(fits);
                }
                Eva = 0;
            }
        }
    }
    return evaluation;
}

Fit::DotORLine AlgorithmWrapper::findEnd(procon::ExpandedPolygon polygon1, procon::ExpandedPolygon polygon2,int &comp1,int &comp2, int &Eva)
{
    double comped;
    double comping;
    for (int r = 0; r<polygon1.getSize(); r++){
        //decrement comp1
        comp1--;
        if (comp1 < 0) {
            comp1=polygon1.getSize()-1;
        }
        //compare LENGTH comp1 <=> comp2
        comped=polygon1.getSideLength()[comp1];
        comping=polygon2.getSideLength()[comp2];
        if (comped - (length_error * 2) < comping && comping < comped + (length_error * 2)){
            //increment Eva
            Eva++;
        } else {
            //increment comp1
            if (comp1 == polygon1.getSize() - 1){
                comp1--;
            }
            comp1++;
            //return
            return Fit::Dot;

        }

        //increment comp2
        comp2++;
        if (comp2 > polygon2.getSize()-1){
            comp2=0;
        }
        //compare ANGLE comp1 <=> comp2
        comped=polygon1.getSideAngle()[comp1];
        comping=polygon2.getSideAngle()[comp2];
        if ((M_PI * 2) - angle_error * 2 < (comping + comped) && (comping + comped) < (M_PI * 2) + angle_error * 2){
            //increment Eva
            Eva++;
        } else {
            //decrement comp2
            if (comp2 == 0){
                comp2=polygon2.getSize();
            }
            comp2--;
            //return
            return Fit::Line;
        }
    }
    return Fit::Dot;
}

std::vector<Evaluation> AlgorithmWrapper::evaluateCombination(procon::ExpandedPolygon frame, procon::ExpandedPolygon piece)
{

    auto findEndPoint = [&](int k,int & comp1,int & comp2,int & Eva)->Fit::DotORLine{
        double comp_frame;
        double comp_piece;
        const int frame_size = static_cast<int>(frame.getPolygon().inners().at(k).size() - 1);
        for (int r = 0; r < frame_size; r++){
            //decrement comp1
            comp1--;
            if (comp1 < 0) {
                comp1 = frame_size - 1;
            }
            //compare LENGTH comp1 <=> comp2
            comp_frame = frame.getInnersSideLength().at(k).at(comp1);
            comp_piece = piece.getSideLength().at(comp2);
            if (comp_frame - (length_error * 2) < comp_piece && comp_piece < comp_frame + (length_error * 2)) {
                //increment Eva
                Eva++;
            } else {
                //increment comp1
                if (comp1 == frame_size - 1){
                    comp1--;
                }
                comp1++;
                //return
                return Fit::Dot;

            }

            //increment comp2
            comp2++;
            if (comp2 > piece.getSize() - 1){
                comp2 = 0;
            }
            //compare ANGLE comp1 <=> comp2
            comp_frame = frame.getInnersSideAngle().at(k).at(comp1);
            comp_piece = piece.getSideAngle().at(comp2);
            if ((M_PI * 2) - angle_error * 2 < (comp_piece + comp_frame) && (comp_piece + comp_frame) < (M_PI * 2) + angle_error * 2){
                //increment Eva
                Eva++;
            } else {
                //decrement comp2
                if (comp2 == 0){
                    comp2 = piece.getSize();
                }
                comp2--;
                //return
                return Fit::Line;
            }
        }
        return Fit::Dot;
    };

    double frame_first = 0;
    double piece_first = 0;
    int Eva = 0;
    std::vector<Evaluation> evaluations;
    std::array<Fit,2> fits;

    for (int k = 0; k < frame.getPolygon().inners().size(); ++k){
        const int inner_size = static_cast<int>(frame.getPolygon().inners().at(k).size() - 1);
        for (int i = 0; i < inner_size; ++i) {
            frame_first = frame.getInnersSideAngle().at(k).at(i);
            for (int j = 0; j < piece.getSize(); ++j) {
                piece_first = piece.getSideAngle()[j];

                if(procon::nearlyEqual(frame_first,piece_first,angle_error)){

                    Eva++;

                    int start_frame = i;
                    int start_piece = j;
                    Fit::DotORLine dot_or_line = findEndPoint(k,start_frame,start_piece,Eva);
                    fits.at(0).end_dot_or_line = dot_or_line;
                    fits.at(0).end_id = start_frame;
                    fits.at(1).end_dot_or_line = dot_or_line;
                    fits.at(1).end_id = start_piece;

                    //重複していたら追加しない

                    bool isDuplicate = false;
                    /*
                    for(auto accept_fits : result){
                        if(fits[0].end_dot_or_line == accept_fits[0].end_dot_or_line &&
                           fits[0].end_id == accept_fits[0].end_id &&
                           fits[1].end_id == accept_fits[1].end_id &&
                           fits[1].end_dot_or_line == accept_fits[1].end_dot_or_line){
                            isDuplicate = true;
                            break;
                        }
                    }
                    */

                    if(!isDuplicate){
                        start_frame = i;
                        start_piece = j;
                        dot_or_line = findEndPoint(k, start_frame, start_piece, Eva);
                        fits.at(0).start_dot_or_line = dot_or_line;
                        fits.at(0).start_id=start_frame;
                        fits.at(1).start_dot_or_line = dot_or_line;
                        fits.at(1).start_id=start_piece;

                        Evaluation tmp_eva;
                        tmp_eva.evaluation = Eva;
                        tmp_eva.fits = fits;
                        tmp_eva.piece_id = piece.getId();
                        tmp_eva.frame_id = k;
                        evaluations.push_back(tmp_eva);
                    }
                    Eva = 0;
                }
            }
        }
    }
    return evaluations;
}

AlgorithmWrapper::AlgorithmWrapper()
{
}
