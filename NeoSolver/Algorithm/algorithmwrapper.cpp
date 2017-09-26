#include "algorithmwrapper.h"

AlgorithmWrapper::AlgorithmWrapper()
{
    logger = spdlog::get("AlgorithmWrapper");
}


AlgorithmWrapper::~AlgorithmWrapper()
{

}

void AlgorithmWrapper::init(){

}

void AlgorithmWrapper::run(procon::NeoField field)
{
    std::cout << "algorithmwarpper called" << std::endl;
    return;
}

void AlgorithmWrapper::submitAnswer(procon::NeoField field)
{
    std::cout << "algorithm warpper throwanswr called" << std::endl;
    emit throwAnswer(field);
    // Wait 1msec
    QEventLoop loop;
    QTimer::singleShot(1, &loop, SLOT(quit()));
    loop.exec();
}

void AlgorithmWrapper::getCSV()
{
    emit requestCSV();
}

void AlgorithmWrapper::requestCSVcomplete()
{
    logger->info("success");
}

