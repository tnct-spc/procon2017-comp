//#include "mainwindow.h"
#include "neoslaver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::vector<std::shared_ptr<spdlog::logger>> loggers;
    loggers.push_back(spdlog::stdout_color_mt("Kunugida"));
    loggers.push_back(spdlog::stdout_color_mt("ImageRecognition"));
    loggers.push_back(spdlog::stdout_color_mt("Polygon"));
    loggers.push_back(spdlog::stdout_color_mt("Solver"));
    loggers.push_back(spdlog::stdout_color_mt("BeamSearch"));

    for(auto logger : loggers){
        //configre loggers
    }

//    MainWindow w;
//    w.show();
    NeoSlaver s;
    s.show();

    return a.exec();
}
