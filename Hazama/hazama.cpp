#include "hazama.h"
#include "ui_hazama.h"
#include <unistd.h>
#include <iostream>
#include <string>
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
    connect(ui->UseWebCamera, &QCheckBox::stateChanged, this, &Hazama::checkedWebCameraEnable);
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
    char dir[255];
    getcwd(dir,255);
    std::cout<<"Current Directory : "<<dir<<std::endl;

    //Capture
    cv::Mat src;
    std::string path = "./workspace/procon2016-comp/CompImage/dpi300test.png";

    if(WebcameraEnable){
       src = capture();
    }else{
       int state = system("sh ./workspace/procon2016-comp/scanimage.sh");

       if(state ^= 0){
           std::cout << "faild" << std::endl;
       }
       //if checkbox(Use Webcamera) isn`t true
       src = cv::imread(path);
    }

    //Display picture
    cv::namedWindow("picture",CV_WINDOW_AUTOSIZE);
    cv::imshow("picture",src);

    /*
    if ( src.empty() ) {
        std::cerr << "Image can't be loaded!" << std::endl;
    }

    */
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

void Hazama::checkedWebCameraEnable()
{
    std::cout << "checked!" << std::endl;

    WebcameraEnable =  !WebcameraEnable;
}
