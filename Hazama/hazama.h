#ifndef HAZAMA_H
#define HAZAMA_H

#include "answerboard.h"
#include "answerdock.h"
#include "http/request_mapper.h"
#include "field.h"
#include "imagerecognition.h"
#include "polygonio.h"
#include "solver.h"
#include "threshold.h"

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
    Solver* solver;
    ImageRecognition imrec;

    //Calibration
    const std::string calibration_data_file_path = "./../../procon2016-comp/picture/cal/calibration.yml";
    void makeCalibrationData(std::string savefile_path,unsigned int numberOfImages);
    int selectWebCamera();

    //webcamera deviceNumber
    int device_number = 0;


    //Network
    RequestMapper request_mapper;
    procon::Field PDATA;
    bool first_answer_flag = true;
    procon::Field best_answer;

    //threshold gui
    void thresholdGUIinit();
    void disableThresholdUI();
    void enableThresholdUI();

    bool is_pause_flag = false;

private slots:
    void clickedRunButton();
    void thresholdValueChanged();
    void acceptAnswer(QString file_path);


    void clickedApply_1_Button();
    void clickedApply_2_Button();
    void clickedApply_3_Button();
    void clickedApply_4_Button();

    void makeInIFile();
    void loadInIFile();

    void emitAnswer(procon::Field field);

    void pauseClicked();
    void startClicked();

};

#endif // HAZAMA_H
