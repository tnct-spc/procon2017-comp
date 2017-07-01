#include "kunugida.h"
#include "ui_kunugida.h"

#include <iostream>

#include <QDebug>
#include <QPushButton>
#include <QCheckBox>

Kunugida::Kunugida(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Kunugida)
{
    ui->setupUi(this);
    logger = spdlog::get("Kunugida");

    imageRecognitonTest();

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

void Kunugida::imageRecognitonTest()
{
    std::cout << "Hello ImageRecogniton Test" << std::endl;

    cv::Mat nocframe = cv::imread("./../../procon2017-comp/sample/sample_frame_3.JPG", 1);
    cv::Mat nocpieces = cv::imread("/home/spc/ダウンロード/piece3.png", 1);

    ImageRecognition imrec;
    procon::Field PDATA = imrec.run(nocframe, nocpieces);
}
