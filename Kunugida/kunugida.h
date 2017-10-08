#ifndef KUNUGIDA_H
#define KUNUGIDA_H

#include <QMainWindow>
#include "spdlog/spdlog.h"
#include "neoanswerboard.h"
#include "neoanswerdock.h"
#include "imagerecognition.h"
#include "imagerecongnitionwithhumanpower.h"
#include "qrlibrary.h"
#include "neofield.h"
#include "http/request_mapper.h"
#include "qrcode.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QMessageBox>
#include <QTextCodec>

namespace Ui {
class Kunugida;
}

class Kunugida : public QMainWindow
{
    Q_OBJECT

public:
    explicit Kunugida(QWidget *parent = 0);
    void run();
    void imageRecognitonTest();
    ~Kunugida();

signals:
    void requestCSV();
    void requestCSVcomplete();
    void requestpostCSV();

private:
    Ui::Kunugida *ui;
    std::shared_ptr<spdlog::logger> logger;
    bool is_running = false;
    std::shared_ptr<NeoAnswerBoard> board;
    void finishedProcess();
    void startProcess();
    RequestMapper request_mapper;
    QNetworkAccessManager *manager;
    QFile file;
    procon::NeoField PDATA;
    bool first_answer_flag = true;
    procon::NeoField best_answer;
    ImageRecognition imrec;
    procon::NeoField nullfield;

    std::string PROBLEM_SAVE_PATH = "../../procon2017-comp/CSV/problem.csv";
    std::string ANSWER_SAVE_PATH = "../../procon2017-comp/CSV/answer.csv";
public slots:
    void getCSV();
    void postCSV();
    cv::Mat scanImage();

private slots:
    void clickedRunButton();
    void emitAnswer(procon::NeoField field);
    void replyFinished(QNetworkReply* reply);
    void replaceField(procon::NeoField const& field);
    void acceptAnswer();
};

#endif // KUNUGIDA_H
