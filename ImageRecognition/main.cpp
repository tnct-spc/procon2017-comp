#include "imagerecognition.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageRecognition w;
    w.show();
    w.run();

    return a.exec();
}
