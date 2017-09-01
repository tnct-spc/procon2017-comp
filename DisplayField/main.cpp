#include "displayfield.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DisplayField w;
    w.show();

    return a.exec();
}
