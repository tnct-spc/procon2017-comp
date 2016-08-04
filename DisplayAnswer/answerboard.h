#ifndef ANSWERBOARD_H
#define ANSWERBOARD_H

#include <QWidget>
#include "field.h"

namespace Ui {
class AnswerBoard;
}

class AnswerBoard : public QWidget
{
    Q_OBJECT

public:
    explicit AnswerBoard(QWidget *parent = 0);
    ~AnswerBoard();
    void setField(procon::Field& field);

private:
    Ui::AnswerBoard *ui;
    procon::Field field;

protected:
    void paintEvent(QPaintEvent *);
};

#endif // ANSWERBOARD_H
