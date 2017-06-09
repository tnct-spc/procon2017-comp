#include "kunugida.h"
#include "spdlog/spdlog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    std::vector<std::shared_ptr<spdlog::logger>> loggers;
    loggers.push_back(spdlog::stdout_color_mt("Kunugida"));
    loggers.push_back(spdlog::stdout_color_mt("ImageRecognition"));
    loggers.push_back(spdlog::stdout_color_mt("Polygon"));
    loggers.push_back(spdlog::stderr_color_mt("Solver"));

    for(auto logger : loggers){
        //configre loggers
    }

    QApplication a(argc, argv);
    Kunugida w;
    w.show();

    return a.exec();
}
