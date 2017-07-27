#ifndef NEOANSWERDOCK_H
#define NEOANSWERDOCK_H

#include <QWidget>

namespace Ui {
class NeoAnswerDock;
}

class NeoAnswerDock : public QWidget
{
    Q_OBJECT

public:
    explicit NeoAnswerDock(QWidget *parent = 0);
    ~NeoAnswerDock();

private:
    Ui::NeoAnswerDock *ui;
};

#endif // NEOANSWERDOCK_H
