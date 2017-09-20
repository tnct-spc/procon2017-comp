#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include "qrlibrary.h"

namespace Ui {
class MainWindow;
}

using namespace cv;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void Dialogue();
    void Result();
    bool isAuto();
    Mat readimg;
    std::string decodedCode;

private:
    Ui::MainWindow *ui;
    CvCapture* src;
    IplImage* frame;
private slots:
    void takePicture();
    void Quit(int);
};

#endif // MAINWINDOW_H
