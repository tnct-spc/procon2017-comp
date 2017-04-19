#ifndef KUNUGIDA_H
#define KUNUGIDA_H

#include <QMainWindow>

namespace Ui {
class Kunugida;
}

class Kunugida : public QMainWindow
{
    Q_OBJECT

public:
    explicit Kunugida(QWidget *parent = 0);
    void run();
    ~Kunugida();

private:
    Ui::Kunugida *ui;
    bool is_running = false;
    void finishedProcess();
    void startProcess();

private slots:
    void clickedRunButton();
};

#endif // KUNUGIDA_H
