#include "hazama.h"
#include "ui_hazama.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QFileDialog>

#include "field.h"
#include "imagerecognition.h"
#include "polygonio.h"
#include "solver.h"

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
    if(ui->useWebCamera->isChecked() || ui->useImageData->isChecked()){
        cv::Mat flame;
        cv::Mat pieces;
        //get Image
        if(ui->useWebCamera->isChecked()){
            cv::namedWindow("capture",cv::WINDOW_AUTOSIZE);
            cv::Mat pressentertextwindow = cv::Mat::zeros(100,700,CV_8UC3);
            cv::putText(pressentertextwindow, "Please Press Enter",cv::Point(0,100),cv::FONT_HERSHEY_SCRIPT_SIMPLEX,2.4,cv::Scalar(255,255,255),2,CV_AA);
            cv::imshow("capture",pressentertextwindow);
            while(cv::waitKey(0)==13);
            flame = capture();
            while(cv::waitKey(0)==13);
            pieces = capture();
            cv::destroyWindow("capture");

        }else{
            std::string flame_path = QFileDialog::getOpenFileName(this,"input flame picture","./../../procon2016-comp/picture/").toStdString();
            std::string pieces_path = QFileDialog::getOpenFileName(this,"input pieces picture","./../../procon2016-comp/picture/").toStdString();
            flame = cv::imread(flame_path, 1);
            pieces = cv::imread(pieces_path, 1);
        }

        /*Image Recognition*/
        ImageRecognition imrec;
        PDATA = imrec.run(flame, pieces);
        //display recognized image
        board->setRawPicture(imrec.getRawPiecesPic(), imrec.getRawPiecesPos());
        board->setRandomColors(imrec.getRawRandomColors());
    }else if(ui->useFileData->isChecked()){
        std::string path = QFileDialog::getOpenFileName(this).toStdString();
        PDATA = procon::PolygonIO::importPolygon(path);
    }else{
        return;
    }

    /*Solve puzzle*/
    Solver solver;
    procon::Field field = solver.run(PDATA);

    /*Display answer*/
    board->setField(field);

    std::cout<<"finish"<<std::endl;
}

cv::Mat Hazama::capture()
{
    cv::VideoCapture cap(1);//デバイスのオープン
    //cap.open(0);//こっちでも良い．

    if(!cap.isOpened()){
        std::cerr << "Can't open camera!" << std::endl;
    }

    //setting
    cap.set(cv::CAP_PROP_FRAME_WIDTH,1920);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT,1080);
    cap.set(cv::CAP_PROP_BRIGHTNESS,0.0);

    cv::Mat src;

    for(int i=0;i<30;i++){
        cap >> src;
    }

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
