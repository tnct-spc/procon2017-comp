#include "kunugida.h"
#include "ui_kunugida.h"
#include "qrlibrary.h"
#include "neofield.h"
#include "probmaker.h"

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <cstdlib>

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

    //スキャナのデバイス名を取得
    char command[256]="sh ../../procon2017-comp/Kunugida/getdevicename.sh";
    if(system(command)==0){
        //うまくデバイス名を取得できたときの処理
        std::cout<<"デバイス名取得できた"<<std::endl;
    }else{
        //デバイス名を取得できなかったときの処理
        std::cout<<"デバイス名取得できない"<<std::endl;
    }
}

Kunugida::~Kunugida()
{
    system("echo > devicename.txt");
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

        cv::Mat first_scan = scanImage();
        cv::Mat second_scan;
        if(!first_scan.empty()){
            cv::imshow("いっこめ",first_scan);
            QMessageBox message_box;
            message_box.setText("スキャン実行");
            QPushButton *button0 = message_box.addButton(tr("cansel"),QMessageBox::ActionRole);
            QPushButton *button1 = message_box.addButton(tr("start"),QMessageBox::ActionRole);
            message_box.exec();
            if(message_box.clickedButton() == button0){
                //キャンセル時
            }else if(message_box.clickedButton() == button1){
                //二回目実行時
                second_scan = scanImage();
                cv::imshow("にこめ",second_scan);
            }
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

cv::Mat Kunugida::scanImage()
{
    time_t t = time(NULL);
    char fileName[256];
    strcpy(fileName , ctime(&t));
    for(char &c : fileName) if(c == ' ' || c == '\n') c = '_';
    strcat(fileName , ".png");
    std::cout<<fileName<<std::endl;

    char command[256]="sh ../../procon2017-comp/Kunugida/getimage.sh ";
    strcat(command , fileName);

    cv::Mat mat;
    if(system(command)==0){
        //うまくスキャンしたときの処理
        std::cout<<"スキャンできた"<<std::endl;
        mat = cv::imread(fileName);
    }else{
        //残念ながらスキャンできなかったときの処理
        std::cout<<"スキャンできない"<<std::endl;
    }
    return mat;
}
