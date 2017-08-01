#ifndef BEAMSEARCH_H
#define BEAMSEARCH_H

#include "algorithmwrapper.h"
#include "spdlog/spdlog.h"

class Connect
{
public:
    int frame_side_num = -1;
    int polygon_side_num = -1;

    int frame_point_num = -1;
    int polygon_point_num = -1;

};

class LightField
{
public:
    std::vector<polygon_i> frame;
    std::vector<polygon_i> pieces;
    std::array<bool,50> is_placed;
};

class BeamSearch : public AlgorithmWrapper
{
public:
    BeamSearch();
    void init() override;
    void run(procon::NeoField field) override;

private:
    std::shared_ptr<spdlog::logger> logger;
};

#endif // BEAMSEARCH_H
