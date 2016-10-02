#ifndef ANSWERDOCK_H
#define ANSWERDOCK_H

#include <QWidget>

#include "field.h"
#include "answerboard.h"

namespace Ui {
class AnswerDock;
}

class AnswerDock : public QWidget
{
    Q_OBJECT

public:
    explicit AnswerDock(QWidget *parent = 0);
    ~AnswerDock();
    void addAnswer(procon::Field const& field);

private:
    Ui::AnswerDock *ui;
    int answers_size = 0;
};

#endif // ANSWERDOCK_H
