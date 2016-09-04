#include "algorithmwrapper.h"

AlgorithmWrapper::AlgorithmWrapper()
{

}

procon::Field AlgorithmWrapper::run(procon::Field field)
{
    return field;
}

int AlgorithmWrapper::searchSameLength(procon::ExpandedPolygon polygon1, procon::ExpandedPolygon polygon2, std::vector<std::array<Fit,2>> &result)
{
    /*許容誤差指定*/
    constexpr double length_error = 0.5; // 単位CM
    constexpr double angle_error = 0.034; //単位rad 0.017rad=1°
    double comped = 0;
    double comping = 0;
    double comped_first = 0;
    double comping_first = 0;
    int Eva = 0;
    int maxEva = 0;
    std::array<Fit,2> fits;
    for (int i = 0; i < polygon1.getSize(); ++i) { //角度のみ探索
        comped_first = polygon1.getSideAngle()[i];
        for (int j = 0; j < polygon2.getSize(); ++j) {
            comping_first = polygon2.getSideAngle()[j];
            if ((M_PI * 2)-(angle_error * 2) < (comped_first + comping_first) && (comped_first + comping_first) < (M_PI * 2)+(angle_error * 2)) {
                std::cout << i << ',' << j << "::" << comped_first + comping_first << std::endl;
                Eva++;
                int start_polygon1 = i;
                int start_polygon2 = j;
                for (int k=0; k<polygon1.getSize(); ++k) {
                    start_polygon2--;
                    if (start_polygon2 < 0) {
                        start_polygon2=polygon2.getSize();
                    }
                    comped = polygon1.getSideLength()[start_polygon1];
                    comping = polygon2.getSideLength()[start_polygon2];
                    if (comped - (length_error*2) < comping && comping < comped + (length_error*2)) {
                        Eva++;
                    } else {
                        if (start_polygon2 == polygon2.getSize()) {
                            start_polygon2 = -1;
                        }
                        fits.at(0).start_dot_or_line = Dot;
                        fits.at(0).start_id=start_polygon1;
                        fits.at(1).start_dot_or_line = Dot;
                        fits.at(1).start_id=start_polygon2+1;

                        break;
                    }

                    start_polygon1++;
                    if (start_polygon1 > polygon1.getSize()) {
                        start_polygon1=0;
                    }
                    comped=polygon1.getSideAngle()[start_polygon1];
                    comping=polygon2.getSideAngle()[start_polygon2];
                    if (comped - (length_error*2) < comping && comping < comped + (length_error*2)) {
                        Eva++;
                    } else {
                        if (start_polygon1 == 0) {
                            start_polygon1=polygon1.getSize()+1;
                        }
                        fits.at(0).start_dot_or_line = Line;
                        fits.at(0).start_id=start_polygon1-1;
                        fits.at(1).start_dot_or_line = Line;
                        fits.at(1).start_id=start_polygon2;

                        break;
                    }
                }

                start_polygon1 = i;
                start_polygon2 = j;

                for (int k=0; k<polygon1.getSize(); ++k){
                    start_polygon1--;
                    if (start_polygon1 < 0) {
                        start_polygon1=polygon1.getSize();
                    }
                    comped=polygon1.getSideLength()[start_polygon1];
                    comping=polygon2.getSideLength()[start_polygon2];
                    if (comped - (length_error*2) < comping && comping < comped + (length_error*2)){
                        Eva++;
                    } else {
                        if (start_polygon1 == polygon1.getSize()){
                            start_polygon1=-1;
                        }

                        if (Eva < 2){
                            Eva = 0;
                        } else {
                            fits.at(0).end_dot_or_line = Dot;
                            fits.at(0).end_id=start_polygon1+1;
                            fits.at(1).end_dot_or_line = Dot;
                            fits.at(1).end_id=start_polygon2;
                            result.push_back(fits);
                        }
                        if (Eva > maxEva){
                            maxEva = Eva;
                        }
                        Eva = 0;
                        break;
                    }


                    start_polygon2++;
                    if (start_polygon2 > polygon2.getSize()){
                        start_polygon2=0;
                    }
                    comped=polygon1.getSideAngle()[start_polygon1];
                    comping=polygon2.getSideAngle()[start_polygon2];
                    if (comped - (length_error*2) < comping && comping < comped + (length_error*2)){
                        Eva++;
                    } else {
                        if (start_polygon2 == 0){
                            start_polygon2=polygon2.getSize()+1;
                        }

                        if (Eva < 2){
                            Eva = 0;
                        } else {
                            fits.at(0).end_dot_or_line = Line;
                            fits.at(0).end_id=start_polygon1;
                            fits.at(1).end_dot_or_line = Line;
                            fits.at(1).end_id=start_polygon2-1;
                            result.push_back(fits);
                        }
                        if (Eva > maxEva){
                            maxEva = Eva;
                        }
                        Eva = 0;
                        break;
                    }
                }
            }
        }
    }
    return maxEva;
}
