#include "kunugida.h"
#include "ui_kunugida.h"

Kunugida::Kunugida(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Kunugida)
{
    ui->setupUi(this);
}

Kunugida::~Kunugida()
{
    delete ui;
}
