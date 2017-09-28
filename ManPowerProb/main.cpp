#include "manpowerprob.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ManPowerProb w;
    w.show();

    return a.exec();
}
