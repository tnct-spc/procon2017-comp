#ifndef KUNUGIDA_H
#define KUNUGIDA_H

#include <QMainWindow>
#include "spdlog/spdlog.h"
#include "neoanswerboard.h"
#include "neoanswerdock.h"
#include "imagerecognition.h"
#include "neofield.h"
#include "http/request_mapper.h"
#include "tcpserver.h"

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
    std::shared_ptr<TcpServer> server;
    void finishedProcess();
    void startProcess();

    // Network
    RequestMapper request_mapper;
    procon::NeoField PDATA;
    bool first_answer_flag = true;
    procon::NeoField best_answer;

private slots:
    void clickedRunButton();
    void emitAnswer(procon::NeoField field);
};

#endif // KUNUGIDA_H
