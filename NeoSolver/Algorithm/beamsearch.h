#ifndef BEAMSEARCH_H
#define BEAMSEARCH_H

#include "algorithmwrapper.h"
#include "spdlog/spdlog.h"
#include "Utils/polygonconnector.h"
#include "Evaluation/evaluation.h"
#include "neoanswerdock.h"

class Evaluate {
public:
    double score = -1;
    Connect connection;
    int frame_index = -1;
    int piece_index = -1;
    int fields_index = -1;
    bool is_inversed = false;
};

class Connect
{
public:
    //接するべき辺の若い方のpointのindex
    int frame_side_num = -1;
    int polygon_side_num = -1;

    //触れてるべき頂点のindex
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
    void makeNextState(std::vector<procon::NeoField> & fields,std::vector<Evaluate> & evaluations);
    void evaluateNextState(std::vector<procon::NeoField> & fields,std::vector<Evaluate> & evaluations);
    bool checkCanPrune(procon::NeoField const& field);
    std::vector<procon::NeoField> debug_field;

private:
    std::shared_ptr<spdlog::logger> logger;
    std::shared_ptr<NeoAnswerDock> dock;
    std::shared_ptr<NeoAnswerDock> neo;
    unsigned int processor_num = 0;
};

#endif // BEAMSEARCH_H
