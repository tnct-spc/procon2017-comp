#include "manpowerprob.h"
#include "manpowergui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    std::vector<std::shared_ptr<spdlog::logger>> loggers;
    loggers.push_back(spdlog::stdout_color_mt("ManPowerProb"));

    QApplication a(argc, argv);
    ManPowerGui w;

    w.show();

    return a.exec();
}
