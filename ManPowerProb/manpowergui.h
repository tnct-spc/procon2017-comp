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
    unsigned int spin_x=0;
    unsigned int spin_y=0;
};

#endif // MANPOWERGUI_H
