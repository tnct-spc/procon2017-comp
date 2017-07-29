#ifndef ALGORITHMWRAPPER_H
#define ALGORITHMWRAPPER_H

#include "neofield.h"

class AlgorithmWrapper : public QObject
{
    Q_OBJECT

public:
    AlgorithmWrapper();
    ~AlgorithmWrapper();
    void init();
    void run(procon::NeoField field);
    void submitAnswer(procon::NeoField field);

signals:
    void throwAnswer(procon::NeoField field);
};

#endif // ALGORITHMWRAPPER_H
