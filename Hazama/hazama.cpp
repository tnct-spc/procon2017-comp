#include "hazama.h"
#include "ui_hazama.h"

#include <iostream>

Hazama::Hazama(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Hazama)
{
    ui->setupUi(this);
    connect(ui->RunButton, &QPushButton::clicked, this, &Hazama::clickedRunButton);
}

Hazama::~Hazama()
{
    delete ui;
}

void Hazama::init()
{

}

void Hazama::run()
{
    std::cout << "Run" << std::endl;

    /*Take a picture*/
    capture();

    /*Image Recognition*/

    /*Solve puzzle*/

    /*Display answer*/

}

void Hazama::capture()
{

}

void Hazama::clickedRunButton()
{
    run();
}
