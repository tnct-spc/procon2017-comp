#ifndef BEAMSEARCH_H
#define BEAMSEARCH_H

#include "algorithmwrapper.h"
#include "spdlog/spdlog.h"

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
