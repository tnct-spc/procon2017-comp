#ifndef KUNUGIDA_H
#define KUNUGIDA_H

#include <QMainWindow>
#include "spdlog/spdlog.h"
#include "neoanswerboard.h"
#include "imagerecognition.h"

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

private slots:
    void clickedRunButton();
};

#endif // KUNUGIDA_H
