#ifndef HAZAMA_H
#define HAZAMA_H

#include <QMainWindow>

namespace Ui {
class Hazama;
}

class Hazama : public QMainWindow
{
    Q_OBJECT

public:
    explicit Hazama(QWidget *parent = 0);
    ~Hazama();

private:
    Ui::Hazama *ui;
    void init();
    void run();
    void capture();
private slots:
    void clickedRunButton();
};

#endif // HAZAMA_H
