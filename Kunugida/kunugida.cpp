#include "kunugida.h"
#include "ui_kunugida.h"

#include <iostream>

#include <QPushButton>

Kunugida::Kunugida(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Kunugida)
{
    ui->setupUi(this);

    connect(ui->RunButton, &QPushButton::clicked, this, &Kunugida::clickedRunButton);
}

Kunugida::~Kunugida()
{
    delete ui;
}

void Kunugida::clickedRunButton()
{
    std::cout << "Clicked Run Button" << std::endl;
}
