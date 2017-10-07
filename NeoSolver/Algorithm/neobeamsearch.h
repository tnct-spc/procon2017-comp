#ifndef NEOBEAMSEARCH_H
#define NEOBEAMSEARCH_H

#include "neofield.h"
#include "Algorithm/algorithmwrapper.h"
#include "neoanswerboard.h"
#include "neoanswerdock.h"


class NeoBeamSearch : public AlgorithmWrapper
{
public:
    NeoBeamSearch();
    NeoBeamSearch(int beamwidth);
    NeoBeamSearch(int beamwidth,bool answerprogress_enabled);
    void init() override;
    void run(procon::NeoField field) override;
    void makeNextState(std::vector<procon::NeoField> & fields,std::vector<Evaluate> & evaluations);
    void evaluateNextState(std::vector<procon::NeoField> & fields,std::vector<Evaluate> & evaluations);
    bool checkCanPrune(procon::NeoField const& field);
    int checkOddField(const procon::NeoField &field, const Connect &connector, const int field_frame_index, const int field_piece_index);

    std::vector<procon::NeoField> debug_field;

    static std::string hashField(const procon::NeoField& field);

    void setBeamWidth(int beamwidth);

private:
    std::shared_ptr<spdlog::logger> logger;
    std::shared_ptr<NeoAnswerDock> dock;
    std::shared_ptr<NeoAnswerDock> neo;
    std::shared_ptr<NeoAnswerDock> last_selector;

    std::vector<procon::NeoField> last_fields;
    bool answer_progress_enabled = true;

    unsigned int cpu_num = 0;
    unsigned int beam_width = 20;

private slots:
    void tryNextBeamSearch(procon::NeoField next_field);
};

#endif // NEOBEAMSEARCH_H
