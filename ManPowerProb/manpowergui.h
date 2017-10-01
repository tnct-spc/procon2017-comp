#ifndef MANPOWERGUI_H
#define MANPOWERGUI_H

#include <QWidget>
#include <manpowerprob.h>



namespace Ui {
class ManPowerGui;
}

class ManPowerGui : public QWidget
{
    Q_OBJECT

public:
    explicit ManPowerGui(QWidget *parent = 0);
    ~ManPowerGui();

signals:
    void zoom_In();
    void zoom_Out();
    QString piece_Changed(QString value);


private:
    Ui::ManPowerGui *ui;
    QGraphicsView *view;
    QGraphicsScene *scene;
    ManPowerProb *ManProb;
    unsigned int spin_x=0;
    unsigned int spin_y=0;
    unsigned int center_x=0;
    unsigned int center_y=0;
    double zoom=1;
    void zoomIn(double value);

private slots:
    void _zoom_In();
    void _zoom_Out();
};

#endif // MANPOWERGUI_H
