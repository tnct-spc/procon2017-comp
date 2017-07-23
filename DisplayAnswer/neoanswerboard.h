#ifndef NEOANSWERBOARD_H
#define NEOANSWERBOARD_H

#include <QWidget>

namespace Ui {
class NeoAnswerBoard;
}

class NeoAnswerBoard : public QWidget
{
    Q_OBJECT

public:
    explicit NeoAnswerBoard(QWidget *parent = 0);
    ~NeoAnswerBoard();

private:
    Ui::NeoAnswerBoard *ui;

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // NEOANSWERBOARD_H
