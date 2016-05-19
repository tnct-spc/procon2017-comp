#include "probmaker.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ProbMaker w;
    w.show();
    w.run();

    return a.exec();
}
