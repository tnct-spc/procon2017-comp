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
#include "displayanswer.h"
#include "solver.h"
#include "imagerecognition.h"

Hazama::Hazama(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Hazama)
{
    ui->setupUi(this);
    connect(ui->RunButton, &QPushButton::clicked, this, &Hazama::clickedRunButton);
}

Hazama::~Hazama()
{
    delete ui;
}

void Hazama::init()
{

}

cv::Mat drawRawPicWithId(cv::Mat raw_pieces_pic, std::vector<cv::Point> raw_pieces_pos)
{
    for(int i=0;i<raw_pieces_pos.size();++i){
        cv::putText(raw_pieces_pic, std::to_string(i), cv::Point(raw_pieces_pos[i].x-60,raw_pieces_pos[i].y+60), cv::FONT_HERSHEY_SIMPLEX, 5, cv::Scalar(255,0,0));
    }
    return raw_pieces_pic;
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
        cv::Mat raw_pieces_pic = imrec.getRawPiecesPic();
        std::vector<cv::Point> raw_pieces_pos = imrec.getRawPiecesPos();
        cv::Mat disp_pic = drawRawPicWithId(raw_pieces_pic,raw_pieces_pos);
        cv::namedWindow("an",CV_WINDOW_NORMAL);
        cv::imshow("an", raw_pieces_pic);

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
    Display.setField(field);
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
