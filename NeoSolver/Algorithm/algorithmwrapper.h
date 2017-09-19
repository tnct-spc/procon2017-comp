#ifndef ALGORITHMWRAPPER_H
#define ALGORITHMWRAPPER_H

#include "neofield.h"
#include "spdlog/spdlog.h"

class AlgorithmWrapper : public QObject
{
    Q_OBJECT

public:
    AlgorithmWrapper();
    ~AlgorithmWrapper();
    virtual void init();
    virtual void run(procon::NeoField field);
    void submitAnswer(procon::NeoField field);

private:
    void getCSV();
    std::shared_ptr<spdlog::logger> logger;

public slots:
    void requestCSVcomplete();

signals:
    void throwAnswer(procon::NeoField field);
    void requestCSV();
};

#endif // ALGORITHMWRAPPER_H
