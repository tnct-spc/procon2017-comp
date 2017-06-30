#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>

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
    Mat readimg;
private:
    Ui::MainWindow *ui;
    CvCapture* src;
    IplImage* frame;

private slots:
    void takePicture();
    void closeWindow();
};

#endif // MAINWINDOW_H
