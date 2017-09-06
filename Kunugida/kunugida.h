#ifndef KUNUGIDA_H
#define KUNUGIDA_H

#include <QMainWindow>
#include "spdlog/spdlog.h"
#include "neoanswerboard.h"
#include "neoanswerdock.h"
#include "imagerecognition.h"
#include "neofield.h"
#include "http/request_mapper.h"
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
    void pleaseCSV();

private slots:
    void clickedRunButton();
    void emitAnswer(procon::NeoField field);
    void replyFinished(QNetworkReply* reply);
};

#endif // KUNUGIDA_H
