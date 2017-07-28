#ifndef NEOSOLVER_H
#define NEOSOLVER_H

#include "neosolver_global.h"
#include "Algorithm/algorithmwrapper.h"
#include "neofield.h"

#include <QObject>

class NEOSOLVERSHARED_EXPORT NeoSolver : public QObject
{
    Q_OBJECT

public:
    NeoSolver();
    ~NeoSolver();
    void run(procon::NeoField field,int algorithm_number);

private:
    std::vector<AlgorithmWrapper*> Algorithms;

signals:
    void throwAnswer(procon::NeoField field);

};

#endif // NEOSOLVER_H
