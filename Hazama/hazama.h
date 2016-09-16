#ifndef HAZAMA_H
#define HAZAMA_H

#include <QMainWindow>
#include <opencv2/core/core.hpp>
#include "answerboard.h"

namespace Ui {
class Hazama;
}

class Hazama : public QMainWindow
{
    Q_OBJECT

public:
    explicit Hazama(QWidget *parent = 0);
    ~Hazama();
    void init();
    void run();

private:
    Ui::Hazama *ui;
    cv::Mat capture(int deviceNumber);
    std::shared_ptr<AnswerBoard> board;
    void makeCalibrationData(std::string savefile_path,unsigned int numberOfImages);
    int selectWebCamera();

    //webcamera deviceNumber[
    int device_number = 0;
private slots:
    void clickedRunButton();
};

#endif // HAZAMA_H
