#include "qrcodereader.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QRcodereader w;
    w.show();

    return a.exec();
}
