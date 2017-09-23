#ifndef NEOSOLVER_H
#define NEOSOLVER_H

#include "neosolver_global.h"
#include "Algorithm/algorithmwrapper.h"
#include "neofield.h"
#include "spdlog/spdlog.h"

#include <QObject>
#include <QApplication>

class NEOSOLVERSHARED_EXPORT NeoSolver : public QObject
{
    Q_OBJECT

public:
    NeoSolver();
    ~NeoSolver();
    void run(procon::NeoField field,int algorithm_number);

private:
    std::vector<AlgorithmWrapper*> Algorithms;
    std::shared_ptr<spdlog::logger> logger;
    AlgorithmWrapper* Algorithm;

signals:
    void throwAnswer(procon::NeoField field);
    void requestCSV();
    void requestCSVcomplete();

private slots:
    void emitAnswer(procon::NeoField field);

public slots:
    void _requestCSV();
    void _requestCSVcomplete();
};

#endif // NEOSOLVER_H
