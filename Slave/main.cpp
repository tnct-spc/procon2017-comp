#include "mainwindow.h"
#include "neoslaver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    NeoSlaver s;
    s.show();

    return a.exec();
}
