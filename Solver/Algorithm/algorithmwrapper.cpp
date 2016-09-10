#include "algorithmwrapper.h"
#include "utilities.h"

#include <math.h>

#define PI 3.141592

procon::Field AlgorithmWrapper::run(procon::Field field)
{
    return field;
}

int AlgorithmWrapper::searchSameLength(procon::ExpandedPolygon polygon1, procon::ExpandedPolygon polygon2, std::vector<std::array<Fit,2>> &result)
{
    /*許容誤差指定*/
    constexpr double length_error = 0.05; // 単位CM
    constexpr double angle_error = 0.017; //単位rad 0.017rad=1°
    double comped_first = 0;
    double comping_first = 0;
    int Eva = 0;
    int maxEva = 0;
    std::array<Fit,2> fits;
    for (int i = 0; i < polygon1.getSize(); ++i) { //角度のみ探索
        comped_first = polygon1.getSideAngle()[i];
        for (int j = 0; j < polygon2.getSize(); ++j) {
            comping_first = polygon2.getSideAngle()[j];
            if (procon::nearlyEqual(comped_first,comping_first,angle_error)) {
                Eva++;

                int start_polygon1 = i;
                int start_polygon2 = j;
                AlgorithmWrapper::findEnd(polygon2, polygon1, start_polygon2, start_polygon1,length_error, angle_error, Eva);
                fits.at(0).start_dot_or_line = Fit::Line;
                fits.at(0).start_id=start_polygon1;
                fits.at(1).start_dot_or_line = Fit::Line;
                fits.at(1).start_id=start_polygon2;


                start_polygon1 = i;
                start_polygon2 = j;
                AlgorithmWrapper::findEnd(polygon1, polygon2, start_polygon1, start_polygon2,length_error, angle_error, Eva);
                fits.at(0).end_dot_or_line = Fit::Line;
                fits.at(0).end_id=start_polygon1;
                fits.at(1).end_dot_or_line = Fit::Line;//Now, It has no meaning (For aborting more bug.)
                fits.at(1).end_id=start_polygon2;
                if (Eva < 2){
                    Eva = 0;
                } else {
                    result.push_back(fits);
                }

                if (Eva > maxEva){
                    maxEva = Eva;
                }
                Eva = 0;
            }
        }
    }
    return maxEva;
}

void AlgorithmWrapper::findEnd(procon::ExpandedPolygon polygon1, procon::ExpandedPolygon polygon2,int &comp1,int &comp2, double length_error, double angle_error, int &Eva)
{
    double comped;
    double comping;
    for (int r=0; r<polygon1.getSize(); r++){
        comp1--;
        if (comp1 < 0) {
            comp1=polygon1.getSize()-1;
        }
        comped=polygon1.getSideLength()[comp1];
        comping=polygon2.getSideLength()[comp2];
        if (comped - (length_error*2) < comping && comping < comped + (length_error*2)){
            Eva++;
        } else {
            if (comp1 == polygon1.getSize()-1){
                comp1=-1;
            }
            comp1++;
            return;

        }

        comp2++;
        if (comp2 > polygon2.getSize()-1){
            comp2=0;
        }
        comped=polygon1.getSideAngle()[comp1];
        comping=polygon2.getSideAngle()[comp2];
        if ((M_PI * 2) - angle_error * 2 < (comping + comped) && (comping + comped) < (M_PI * 2) + angle_error * 2){
            Eva++;
        } else {
            if (comp2 == 0){
                comp2=polygon2.getSize();
            }
            comp2--;
            return;
        }
    }
}

AlgorithmWrapper::AlgorithmWrapper()
{
}
