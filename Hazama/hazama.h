#ifndef HAZAMA_H
#define HAZAMA_H

#include <QMainWindow>
#include <opencv2/core/core.hpp>
#include "answerboard.h"
#include "http/request_mapper.h"

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
    cv::Mat capture();
    std::shared_ptr<AnswerBoard> board;

    //Calibration
    void makeCalibrationData(std::string savefile_path,unsigned int numberOfImages);

    //Network
    RequestMapper request_mapper;
    procon::Field PDATA;
    bool first_answer_flag = true;
    procon::Field best_answer;

private slots:
    void clickedRunButton();
    void acceptAnswer(QString file_path);
};

#endif // HAZAMA_H
