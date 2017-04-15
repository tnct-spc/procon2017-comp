#include "kunugida.h"
#include "ui_kunugida.h"

#include <iostream>

#include <QDebug>
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

void Kunugida::run()
{
    std::cout << "Run" << std::endl;
    this->finishedProcess();
}

void Kunugida::clickedRunButton()
{
    if(!this->is_running){
        std::cout << "Start Process" << std::endl;
        this->startProcess();
        this->run();
    }else{
        std::cout << "Main Process is Already Running" << std::endl;
    }
}

void Kunugida::finishedProcess()
{
    this->is_running = false;
}

void Kunugida::startProcess()
{
    this->is_running = true;
}
