#include "kunugida.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Kunugida w;
    w.show();

    return a.exec();
}
