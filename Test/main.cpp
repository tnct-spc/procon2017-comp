#include <iostream>

#include <QApplication>

#include "field.h"

#include "testdisplayanswer.h"

#include <math.h>

#define PI 3.141592

bool Test()
{
    TestDisplayAnswer test_displayanswer;

    if(!test_displayanswer.run()){
        std::cout<<"*****Display error*****"<<std::endl;
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    printf("ok");
    QApplication a(argc, argv);

    return a.exec();
}
