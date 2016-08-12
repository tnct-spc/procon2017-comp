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
    cv::Mat capture();
    std::shared_ptr<AnswerBoard> board;
private slots:
    void clickedRunButton();
};

#endif // HAZAMA_H
