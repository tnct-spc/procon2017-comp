#include "kunugida.h"
#include "ui_kunugida.h"

#include <iostream>

#include <QDebug>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>

Kunugida::Kunugida(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Kunugida)
{
    ui->setupUi(this);

    connect(ui->RunButton, &QPushButton::clicked, this, &Kunugida::clickedRunButton);

    connect(ui->run_debug_button, &QPushButton::clicked, this, &Kunugida::clickedDebugRunButton);
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

void Kunugida::clickedDebugRunButton()
{
    if(ui->ui_test->isChecked()){
        std::cout << "UI test" << std::endl;
    }else if(ui->image_recognition_test->isChecked()){
        std::cout << "ImageRecognitionTest" << std::endl;
    }else{
        std::cerr << "Warning Yabai" << std::endl;
    }
}
