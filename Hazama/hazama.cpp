#include "hazama.h"
#include "ui_hazama.h"

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "field.h"
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

    Field field;
    ImageRecognition imrec;

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
    imrec.run();

    /*Solve puzzle*/

    /*Display answer*/
    cv::Mat raw_pieces_pic = imrec.getRawPiecesPic();
    std::vector<cv::Point> raw_pieces_pos = imrec.getRawPiecesPos();
    cv::Mat disp_pic = drawRawPicWithId(raw_pieces_pic,raw_pieces_pos);

    cv::namedWindow("an",CV_WINDOW_NORMAL);
    cv::imshow("an", raw_pieces_pic);

    cv::waitKey();
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
