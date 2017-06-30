#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //各種ボタン
    connect(ui->exitButton, &QPushButton::clicked,
            this, &MainWindow::closeWindow);
    connect(ui->okButton, &QPushButton::clicked,
            this, &MainWindow::takePicture);
    //明るさ設定関連
    connect(ui->brightnessSlider, SIGNAL(valueChanged(int)),
            ui->brightnessSpinBox, SLOT(setValue(int)) );
    connect(ui->brightnessSpinBox, SIGNAL(valueChanged(int)),
            ui->brightnessSlider, SLOT(setValue(int)) );
    //感知面積関連
    connect(ui->measureSlider, SIGNAL(valueChanged(int)),
            ui->measureSpinBox, SLOT(setValue(int)) );
    connect(ui->measureSpinBox, SIGNAL(valueChanged(int)),
            ui->measureSlider, SLOT(setValue(int)) );
    //色関連
    connect(ui->RGBSlider, SIGNAL(valueChanged(int)),
            ui->RGBSpinBox, SLOT(setValue(int)) );
    connect(ui->RGBSpinBox, SIGNAL(valueChanged(int)),
            ui->RGBSlider, SLOT(setValue(int)) );
    //設定の有効化
    connect(ui->ASEnabler, &QCheckBox::toggled,
            ui->advancedSettings, &QGroupBox::setEnabled);

    connect(ui->action_Quit, &QAction::triggered,
            this, &MainWindow::closeWindow);
}

MainWindow::~MainWindow()
{
    delete ui;
}

using namespace cv;

void MainWindow::takePicture()
{
    int key;
    char* LoadedImage = "Press any key...";
    cvNamedWindow(LoadedImage, CV_WINDOW_NORMAL);
    src = cvCaptureFromCAM(0);

    if(src == NULL){

    }
    //カメラが使用可能か
    while(1){
        frame = cvQueryFrame(src);
        readimg = cvarrToMat(frame);
        Mat resizedimg(readimg, Rect(240, 0, 720, 720));
        readimg = resizedimg;
        rectangle(readimg, Point(120, 120), Point(600, 600), CV_RGB(0,0,0), 5);
        imshow(LoadedImage, readimg);
        key = cvWaitKey(16);
        if(key >= 27) break;
    }
    cvReleaseCapture(&src);
    destroyWindow(LoadedImage);
    delete ui;
}

void MainWindow::closeWindow()
{
    delete ui;
}
