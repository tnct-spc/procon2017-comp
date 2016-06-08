#include "hazama.h"
#include "ui_hazama.h"

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "field.h"

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

void Hazama::run()
{
    std::cout << "Run" << std::endl;

    Field field;

    /*Take a picture*/

    //Capture
    cv::Mat src = capture();

    //Display picture
    cv::namedWindow("picture",CV_WINDOW_AUTOSIZE);
    cv::imshow("picture",src);
    if ( src.empty() ) {
        std::cerr << "Image can't be loaded!" << std::endl;
    }

    /*Image Recognition*/

    /*Solve puzzle*/

    /*Display answer*/

}

cv::Mat Hazama::capture()
{
    cv::VideoCapture cap(0);//デバイスのオープン
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
