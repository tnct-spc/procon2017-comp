#include "searchsamelength.h"
#include "Algorithm/algorithmwrapper.h"

std::vector<int> SearchSameLength::evaluateMatching(const procon::ExpandedPolygon& polygon1, const procon::ExpandedPolygon& polygon2, std::vector<std::array<Fit,2>> &result)
{
    auto isStraightAngle = [](double left, double right, bool is_inner_left, bool is_inner_right){
        if(is_inner_left) left = (M_PI * 2) - left;
        if(is_inner_right) right = (M_PI * 2) - right;
        return (M_PI) - AlgorithmWrapper::angle_error * 2 < (left + right) && (left + right) < (M_PI) + AlgorithmWrapper::angle_error * 2;
    };

    auto isMatchAngle = [](double left, double right, bool is_inner_left, bool is_inner_right){
        if(is_inner_left) left = (M_PI * 2) - left;
        if(is_inner_right) right = (M_PI * 2) - right;
        return (M_PI * 2) - AlgorithmWrapper::angle_error * 2 < (left + right) && (left + right) < (M_PI * 2) + AlgorithmWrapper::angle_error * 2;
    };

    auto isMatchLength = [](double left, double right){
        return right - (AlgorithmWrapper::length_error * 2) < left && left < right + (AlgorithmWrapper::length_error * 2);
    };

    std::vector<int> evaluation;

    // フレームならそれぞれのポリゴン同士で、比較

    for(int p1_inner_cnt = 0; p1_inner_cnt < (polygon1.getInnerSize() != 0 ? polygon1.getInnerSize() : 1); ++p1_inner_cnt){

        int polygon1_size = polygon1.getInnerSize() != 0 ? polygon1.getPolygon().inners().at(p1_inner_cnt).size()-1 : polygon1.getSize();
        std::vector<double> polygon1_angles = polygon1.getInnerSize() != 0 ? polygon1.getInnersSideAngle().at(p1_inner_cnt) : polygon1.getSideAngle();
        std::vector<double> polygon1_lengths = polygon1.getInnerSize() != 0 ? polygon1.getInnersSideLength().at(p1_inner_cnt) : polygon1.getSideLength();

        for(int p2_inner_cnt = 0; p2_inner_cnt < (polygon2.getInnerSize() != 0 ? polygon2.getInnerSize() : 1); ++p2_inner_cnt){

            int polygon2_size = polygon2.getInnerSize() != 0 ? polygon2.getPolygon().inners().at(p2_inner_cnt).size()-1 : polygon2.getSize();
            std::vector<double> polygon2_angles = polygon2.getInnerSize() != 0 ? polygon2.getInnersSideAngle().at(p2_inner_cnt) : polygon2.getSideAngle();
            std::vector<double> polygon2_lengths = polygon2.getInnerSize() != 0 ? polygon2.getInnersSideLength().at(p2_inner_cnt) : polygon2.getSideLength();

            // 一致する場所の開始を探して、終了まで探索する

            auto calcSameShape = [&](int p1_pos, int p2_pos, Fit::DotORLine dot_or_line){
                int Eva = 1;
                std::array<Fit, 2> fits;

                fits.at(0).frame_inner_pos = polygon1.getInnerSize() != 0 ? p1_inner_cnt : -1;
                fits.at(1).frame_inner_pos = polygon2.getInnerSize() != 0 ? p2_inner_cnt : -1;
                fits.at(0).start_dot_or_line = dot_or_line;
                fits.at(0).start_id = p1_pos;
                fits.at(1).start_dot_or_line = dot_or_line;
                fits.at(1).start_id = p2_pos;
                fits.at(0).is_start_straight = isStraightAngle(polygon1_angles[p1_pos], polygon2_angles[p2_pos], polygon1.getInnerSize() != 0, polygon2.getInnerSize() != 0);

                auto lengthCheck = [&](){
                    //decrement comp2
                    p2_pos--;
                    if (p2_pos == -1){
                        p2_pos = polygon2_size - 1;
                    }

                    //compare LENGTH p1 <=> p2
                    if(isMatchLength(polygon1_lengths[p1_pos], polygon2_lengths[p2_pos])){
                        Eva++;
                        dot_or_line = Fit::Line;
                        return true;
                    } else {

                        // 終了
                        p2_pos++;
                        if (p2_pos == polygon2_size) {
                            p2_pos = 0;
                        }
                        dot_or_line = Fit::Dot;
                        return false;

                    }
                };

                auto angleCheck = [&](){
                    //increment p1_pos
                    p1_pos++;
                    if (p1_pos == polygon1_size) {
                        p1_pos = 0;
                    }

                    //compare ANGLE comp1 <=> comp2
                    if (isMatchAngle(polygon1_angles[p1_pos], polygon2_angles[p2_pos], polygon1.getInnerSize() != 0, polygon2.getInnerSize() != 0)){
                        //increment Eva
                        Eva++;
                        dot_or_line = Fit::Dot;
                        return true;
                    } else {
                        // 終了
                        p1_pos--;
                        if (p1_pos == -1){
                            p1_pos = polygon1_size - 1;
                        }
                        dot_or_line = Fit::Line;
                        return false;
                    }
                };

                // ひとつずつずらしていく
                for (int r = 0; r < polygon1_size; r++){
                    bool is_continue;
                    if(dot_or_line == Fit::Dot){
                        is_continue = lengthCheck();
                    }else{
                        is_continue = angleCheck();
                    }
                    if(!is_continue) break;
                }

                fits.at(0).end_dot_or_line = dot_or_line;
                fits.at(0).end_id=p1_pos;
                fits.at(1).end_dot_or_line = dot_or_line;
                fits.at(1).end_id=p2_pos;
                fits.at(0).is_end_straight = isStraightAngle(polygon1_angles[p1_pos], polygon2_angles[p2_pos], polygon1.getInnerSize() != 0, polygon2.getInnerSize() != 0);

                //重複していたらcompare erase
                bool is_apply = true;
                for(unsigned int i = 0; i < result.size(); ++i){
                    if( fits[0].frame_inner_pos == result[i][0].frame_inner_pos &&
                        fits[1].frame_inner_pos == result[i][1].frame_inner_pos &&
                        fits[0].end_dot_or_line == result[i][0].end_dot_or_line &&
                        fits[0].end_id == result[i][0].end_id &&
                        fits[1].end_id == result[i][1].end_id &&
                        fits[1].end_dot_or_line == result[i][1].end_dot_or_line){

                        if(Eva > evaluation[i]){
                            result.erase(result.begin()+i);
                            evaluation.erase(evaluation.begin()+i);
                        }else{
                            is_apply = false;
                        }
                        break;
                    }
                }

                if(is_apply){
                    evaluation.push_back(Eva);
                    result.push_back(fits);
                }
                return true;
            };

            for (int i = 0; i < polygon1_size; ++i) {
                int p1_pos = i;
                int p2_pos;
                for (int j = polygon2_size-1; j >= 0; --j, ++p1_pos) {
                    if(p1_pos == polygon1_size) p1_pos = 0;
                    p2_pos = j;
                    if(isMatchAngle(polygon1_angles[p1_pos], polygon2_angles[p2_pos], polygon1.getInnerSize() != 0, polygon2.getInnerSize() != 0)){
                        calcSameShape(p1_pos, p2_pos, Fit::Dot);
                    }
                    if(isMatchLength(polygon1_lengths[p1_pos], polygon2_lengths[p2_pos == 0 ? polygon2_size-1 : p2_pos-1])){
                        calcSameShape(p1_pos, p2_pos == 0 ? polygon2_size-1 : p2_pos-1, Fit::Line);
                    }
                }
            }
        }
    }

    return evaluation;
}
