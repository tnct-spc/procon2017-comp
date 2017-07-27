#include "kunugida.h"
#include "ui_kunugida.h"
#include "qrlibrary.h"
#include "neofield.h"
#include "probmaker.h"

#include <iostream>
#include <stdlib.h>

#include <QDebug>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>

Kunugida::Kunugida(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Kunugida)
{
    ui->setupUi(this);
    logger = spdlog::get("Kunugida");

//    imageRecognitonTest();

    connect(ui->RunButton, &QPushButton::clicked, this, &Kunugida::clickedRunButton);

    board = std::make_shared<NeoAnswerBoard>();
    board->show();
}

Kunugida::~Kunugida()
{
    delete ui;
}

void Kunugida::run()
{
    logger->info("Run Button Clicked");

    procon::NeoField field;

    if(ui->probmaker_button->isChecked()){
        //selected probmaker
        logger->info("Selected ProbMaker DataSource");

        ProbMaker *PbMaker = new ProbMaker();

        //もしProbMakerの結果を表示したければ下をコメントアウト
        PbMaker->show();
        std::vector<polygon_i> pieces = PbMaker->getPieces();
        polygon_i frame = PbMaker->getFrame();

    }else if(ui->scanner_button->isChecked()){
        //selected scanner
        logger->info("Selected Scanner DataSource");

        char command[256]="sh ../../procon2017-comp/Kunugida/getimage.sh";
        cv::Mat mat;
        if(system(command)==0){
            //うまくスキャンしたときの処理
            mat = cv::imread("hoge.png");
            cv::imshow("でででん",mat);
            std::cout<<"できた"<<std::endl;
        }else{
            //残念ながらスキャンできなかったときの処理
            std::cout<<"できない"<<std::endl;
        }
    }else if(ui->image_data_button->isChecked()){
        //selected image
        logger->info("Selected ImageData DataSource");
    }
//    TODO: ここまでで各データソースから読み込むようにする




//    QRLibrary lib;
//    lib.Decoder(true);
    
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
