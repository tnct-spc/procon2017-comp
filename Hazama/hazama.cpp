#include "hazama.h"
#include "ui_hazama.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QFileDialog>

#define noVectored

#include "polygonexpansion.h"
#include "polygonset.h"
#include "field.h"
#include "polygonio.h"
#include "solver.h"
#include "imagerecognition.h"

Hazama::Hazama(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Hazama)
{
    ui->setupUi(this);
    connect(ui->RunButton, &QPushButton::clicked, this, &Hazama::clickedRunButton);
    init();
}

Hazama::~Hazama()
{
    delete ui;
}

void Hazama::init()
{
    board = std::make_shared<AnswerBoard>();
    board->showMaximized();
}

void Hazama::run()
{
    std::cout << "Run" << std::endl;

    procon::Field PDATA;

    /*Get puzzle data*/
    cv::Mat flame;
    cv::Mat pieces;
    if(ui->useWebCamera->isChecked() || ui->useImageData->isChecked()){
        //get Image
        if(ui->useWebCamera->isChecked()){
            flame = capture();
            pieces = capture();
        }else{
            std::string flame_path = QFileDialog::getOpenFileName(this,"input flame picture","./../../procon2016-comp/picture/").toStdString();
            std::string pieces_path = QFileDialog::getOpenFileName(this,"input pieces picture","./../../procon2016-comp/picture/").toStdString();
            flame = cv::imread(flame_path, 1);
            pieces = cv::imread(pieces_path, 1);
        }

        /*Image Recognition*/
        ImageRecognition imrec;
#ifdef noVectored
        imrec.run(flame, pieces);
#else
        PDATA = imrec.run(flame, pieces);
#endif
        //display recognized image
        board->setRawPicture(imrec.getRawPiecesPic(),imrec.getRawPiecesPos());

        //PDATA = ...
    }else if(ui->useFileData->isChecked()){
        std::string path = QFileDialog::getOpenFileName(this).toStdString();
        PDATA = procon::PolygonIO::importPolygon(path);
    }else{
        return;
    }

#ifdef noVectored
    if(!(ui->useWebCamera->isChecked() || ui->useImageData->isChecked())){
#endif
    /*Solve puzzle*/
    Solver solver;
    procon::Field field = solver.run(PDATA);

    /*Display answer*/
    board->setField(field);
#ifdef noVectored
    }
#endif

    std::cout<<"finish"<<std::endl;
}

cv::Mat Hazama::capture()
{
    cv::VideoCapture cap(1);//デバイスのオープン
    //cap.open(0);//こっちでも良い．

    if(!cap.isOpened()){
        std::cerr << "Can't open camera!" << std::endl;
    }

    cv::Mat src;

    cap >> src;

    return src;

    //===convert cv::Mat to QImage===//
    /*
    cv::Mat dst;
    cv::cvtColor(src, dst, CV_RGB2BGR);
    QImage tmp((uchar *)dst.data, dst.cols, dst.rows, dst.step, QImage::Format_RGB888);
    QImage viewImage = tmp.copy();
    */
    //===end===//
}

void Hazama::clickedRunButton()
{
    run();
}
