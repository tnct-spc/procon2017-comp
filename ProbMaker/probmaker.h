#ifndef PROBMAKER_H
#define PROBMAKER_H

#include <QMainWindow>

namespace Ui {
class ProbMaker;
}

class ProbMaker : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProbMaker(QWidget *parent = 0);
    ~ProbMaker();

private:
    Ui::ProbMaker *ui;
};

#endif // PROBMAKER_H
