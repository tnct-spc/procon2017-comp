#ifndef MANPOWERPROB_H
#define MANPOWERPROB_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QWidget>
#include <qpainter.h>

namespace Ui {
class ManPowerProb;
}

class ManPowerProb : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManPowerProb(QWidget *parent = 0);
    ~ManPowerProb();

private:
    Ui::ManPowerProb *ui;

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // MANPOWERPROB_H
