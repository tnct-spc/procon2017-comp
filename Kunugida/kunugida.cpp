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
    logger = spdlog::get("Kunugida");
    connect(ui->RunButton, &QPushButton::clicked, this, &Kunugida::clickedRunButton);
}

Kunugida::~Kunugida()
{
    delete ui;
}

void Kunugida::run()
{
    std::cout << "Run" << std::endl;
    logger->info("Run Button Clicked");
    this->finishedProcess();
}

void Kunugida::clickedRunButton()
{
    if(!this->is_running){
        logger->info("start solving process");
        this->startProcess();
        this->run();
    }else{
        //warning
        logger->warn("solving process is already running");
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
