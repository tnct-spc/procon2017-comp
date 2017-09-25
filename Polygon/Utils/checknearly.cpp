#include "checknearly.h"


CheckNearly::CheckNearly()
{

}

std::vector<procon::ExpandedPolygon> CheckNearly::fit_nearly(const std::vector<procon::ExpandedPolygon> &expolys_d_in, const std::vector<procon::NeoExpandedPolygon> &expolys_i_in)
{
    std::vector<unsigned int> indexs_nearly;
    indexs_nearly.reserve(100);
    auto nearly = [&]()
    {
        for(const auto &expoly_d_in : expolys_d_in) {
            std::vector<double> scores;
            for(const auto &expoly_i_in : expolys_i_in) {
                scores.emplace_back(std::abs(bg::area(expoly_d_in.getPolygon()) - bg::area(expoly_i_in.getPolygon())));
            }

            std::vector<double> scores_sort = scores;
            std::sort(scores_sort.begin(), scores_sort.end());
            if(std::adjacent_find(scores_sort.begin(), scores_sort.end()) != scores_sort.end()) return false;
            indexs_nearly.emplace_back(static_cast<unsigned int>(std::distance(scores.begin(), std::min_element(scores.begin(), scores.end()))));
        }
        std::vector<unsigned int> indexs_sort = indexs_nearly;
        std::sort(indexs_sort.begin(), indexs_sort.end());
        if(std::adjacent_find(indexs_sort.begin(), indexs_sort.end()) != indexs_sort.end()) return false;
        else return true;
    };

    std::vector<unsigned int> indexs_length;
    indexs_length.reserve(100);
    auto length = [&]()
    {
        for(const auto &expoly_d_in : expolys_d_in) {
            std::vector<double> expoly_d_length = expoly_d_in.getSideLength();
            std::vector<double> expoly_d_angle = expoly_d_in.getSideAngle_degree();
            std::vector<double> expoly_d_len_an;
            expoly_d_len_an.reserve(100);
            for(unsigned int a = 0; a != expoly_d_angle.size(); ++a) {
                expoly_d_len_an.emplace_back(expoly_d_angle.at(a));
                expoly_d_len_an.emplace_back(expoly_d_length.at(a));
            }
            bool check1 = false;
            unsigned int index = 0;
            for(const auto &expoly_i_in : expolys_i_in) {
                std::vector<double> expoly_i_length = expoly_i_in.getSideLength();
                std::vector<double> expoly_i_angle = expoly_i_in.getSideAngle_degree();
                std::vector<double> expoly_i_len_an;
                expoly_i_len_an.reserve(100);
                for(unsigned int b = 0; b != expoly_i_angle.size(); ++b) {
                    expoly_i_len_an.emplace_back(expoly_i_angle.at(b));
                    expoly_i_len_an.emplace_back(expoly_i_length.at(b));
                }
                for(unsigned int i = 0; i != expoly_d_len_an.size(); i += 2) {
                    if(i != 0) {
                        bool check2 = true;
                        for(unsigned int j = 0; j != expoly_d_len_an.size(); j += 2) {
                            double angle_d = expoly_d_angle.at(j);
                            double angle_i = expoly_i_angle.at(j);
                            double length_d = expoly_d_length.at(j + 1);
                            double length_i = expoly_i_length.at(j + 1);

                            if(!((angle_i - 9.0) <= angle_d && angle_d <= (length_i + 9.0))) {
                                check2 = false;
                                break;
                            }
                            if(!((length_i - 2) <= length_d && length_d <= (length_i + 2))) {
                                check2 = false;
                                break;
                            }
                            ++j;
                        }
                        if(check2) {
                            check1 = true;
                            break;
                        }
                    }
                    expoly_i_len_an.emplace_back(expoly_i_len_an.at(0));
                    expoly_i_len_an.emplace_back(expoly_i_len_an.at(1));
                    expoly_i_len_an.erase(expoly_i_len_an.begin(), expoly_i_len_an.begin() + 1);
                }
                if(check1) {
                    indexs_length.emplace_back(index);
                    break;
                }
            }
            if(!check1) return false;
        }
        return true;
    };

    std::vector<procon::ExpandedPolygon> expolys_d_out;
    expolys_d_out.reserve(100);
    auto out = [&]()
    {
        unsigned int i = 0;
        for(const auto &index : indexs_nearly) {
            procon::ExpandedPolygon expoly_d_out(expolys_i_in.at(index).getId());
            expoly_d_out.resetPolygonForce(expolys_d_in.at(i).getPolygon());
            expolys_d_out.emplace_back(expoly_d_out);
            ++i;
        }
    };

    if(nearly()) {
        out();
        return expolys_d_out;
    } else if(length()) {
        out();
        return expolys_d_out;
    } else {
        std::vector<procon::ExpandedPolygon> empty_out;
        return empty_out;
    }
}
