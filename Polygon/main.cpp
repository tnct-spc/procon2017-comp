#include "vector2.h"
#include "hoge.h"
#include <QCoreApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::cout << "hey" << std::endl;
    hoge huga;
    huga.print();
    vector2<int> piyo(1,2);
    piyo.print();
    return a.exec();
}
