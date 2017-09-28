#include "manpowerprob.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    std::vector<std::shared_ptr<spdlog::logger>> loggers;
    loggers.push_back(spdlog::stdout_color_mt("ManPowerProb"));

    QApplication a(argc, argv);
    ManPowerProb w;
    w.show();

    return a.exec();
}
