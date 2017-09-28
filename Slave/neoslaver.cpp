#include "neoslaver.h"
#include "ui_neoslaver.h"

neoslaver::neoslaver(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::neoslaver)
{
    ui->setupUi(this);
}

neoslaver::~neoslaver()
{
    delete ui;
}
