#include "probmaker.h"
#include "ui_probmaker.h"

ProbMaker::ProbMaker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProbMaker)
{
    ui->setupUi(this);
}

ProbMaker::~ProbMaker()
{
    delete ui;
}
