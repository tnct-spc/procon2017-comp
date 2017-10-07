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
    void postCSV();
    std::shared_ptr<spdlog::logger> logger;

public slots:
    void requestCSVcomplete();
    void requestpostCSVcomplete();

signals:
    void throwAnswer(procon::NeoField field);
    void requestCSV();
    void requestpostCSV();
};

#endif // ALGORITHMWRAPPER_H
