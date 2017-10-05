#include "kunugida.h"
#include "spdlog/spdlog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    std::vector<std::shared_ptr<spdlog::logger>> loggers;
    loggers.push_back(spdlog::stdout_color_mt("Kunugida"));
    loggers.push_back(spdlog::stdout_color_mt("ImageRecognition"));
    loggers.push_back(spdlog::stdout_color_mt("Polygon"));
    loggers.push_back(spdlog::stdout_color_mt("Solver"));
    loggers.push_back(spdlog::stdout_color_mt("BeamSearch"));

    for(auto logger : loggers){
        //configre loggers
        logger->set_level(spdlog::level::warn);
    }

    unsigned int i = 100;

    std::cout << (-1 < i) << std::endl;

    QApplication a(argc, argv);
    Kunugida w;
    w.show();

    return a.exec();
}
