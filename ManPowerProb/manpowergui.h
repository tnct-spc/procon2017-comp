#ifndef MANPOWERGUI_H
#define MANPOWERGUI_H

#include <QWidget>
#include <manpowerprob.h>
#include <qspinbox.h>



namespace Ui {
class ManPowerGui;
}

class ManPowerGui : public QWidget
{
    Q_OBJECT

public:
    explicit ManPowerGui(QWidget *parent = 0);
    ~ManPowerGui();

private:
    Ui::ManPowerGui *ui;

private slots:
    void deletePiece();
};

#endif // MANPOWERGUI_H
