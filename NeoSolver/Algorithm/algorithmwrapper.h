#ifndef ALGORITHMWRAPPER_H
#define ALGORITHMWRAPPER_H

#include "neofield.h"

class AlgorithmWrapper : public QObject
{
    Q_OBJECT

public:
    AlgorithmWrapper();
    ~AlgorithmWrapper();
    virtual void init();
    virtual void run(procon::NeoField field);
    void submitAnswer(procon::NeoField field);
    std::pair<int, std::vector<procon::NeoExpandedPolygon>> setPieceToFrame(std::vector<std::tuple<int, int, int, int>> tuples);

signals:
    void throwAnswer(procon::NeoField field);
};

#endif // ALGORITHMWRAPPER_H
