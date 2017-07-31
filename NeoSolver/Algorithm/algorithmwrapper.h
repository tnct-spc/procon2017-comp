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
    int evaluation(procon::NeoExpandedPolygon frame, procon::NeoExpandedPolygon polygon);

signals:
    void throwAnswer(procon::NeoField field);
};

#endif // ALGORITHMWRAPPER_H
