#ifndef KUNUGIDA_H
#define KUNUGIDA_H

#include <QMainWindow>
#include "spdlog/spdlog.h"
#include "neoanswerboard.h"
#include "neoanswerdock.h"
#include "imagerecognition.h"
#include "neofield.h"
#include "tcp_main.h"

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
    NeoAnswerDock* list = new NeoAnswerDock;
    std::shared_ptr<TcpMain> tcp;
    void finishedProcess();
    void startProcess();

private slots:
    void clickedRunButton();
    void emitAnswer(procon::NeoField field);
};

#endif // KUNUGIDA_H
