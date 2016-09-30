#ifndef SOLVER_H
#define SOLVER_H

#include "solver_global.h"

#include <QApplication>

#include "field.h"

class SOLVERSHARED_EXPORT Solver : public QObject
{
    Q_OBJECT

public:
    Solver();
    void run(procon::Field field, int algorithm_number = 0);
signals:
    void throwAnswer(procon::Field field);
private slots:
    void emitAnswer(procon::Field field);


};

#endif // SOLVER_H
