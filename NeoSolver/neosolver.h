#ifndef NEOSOLVER_H
#define NEOSOLVER_H

#include "neosolver_global.h"
#include "Algorithm/algorithmwrapper.h"

#include <QObject>

class NEOSOLVERSHARED_EXPORT NeoSolver : public QObject
{
    Q_OBJECT

public:
    NeoSolver();
    ~NeoSolver();

private:
    std::vector<*AlgorithmWrapper> Algorithms;

signals:
    void throwAnswer()

};

#endif // NEOSOLVER_H
