#include "kunugida.h"
#include "spdlog/spdlog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    auto logger = spdlog::stdout_color_mt("Kunugida");

    QApplication a(argc, argv);
    Kunugida w;
    w.show();

    return a.exec();
}
