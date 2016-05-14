#include "hazama.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Hazama w;
    w.show();

    return a.exec();
}
