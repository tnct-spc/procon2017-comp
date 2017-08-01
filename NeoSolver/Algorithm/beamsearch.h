#ifndef BEAMSEARCH_H
#define BEAMSEARCH_H

#include "algorithmwrapper.h"
#include "spdlog/spdlog.h"

class Connect
{
public:
    enum DotOrLine { Dot, Line };

    int start_num;
    int end_num;

    DotOrLine start;
    DotOrLine end;

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
