#include <iostream>

#include <QApplication>

#include "field.h"

#include "testdisplayanswer.h"
#include "utilities.h"
#include <math.h>

#define PI 3.141592

bool Test()
{
    TestDisplayAnswer test_displayanswer;

    if(!test_displayanswer.run()){
        std::cout<<"*****Polygon error*****"<<std::endl;
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    printf("ok");
    QApplication a(argc, argv);

    if(Test()){
        std::cout << "ok" << std::endl;
    } else {
        std::cout << "error" << std::endl;
    }
    return a.exec();
}
