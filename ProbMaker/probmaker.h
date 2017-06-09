#ifndef PROBMAKER_H
#define PROBMAKER_H

#include <QWidget>

namespace Ui {
class ProbMaker;
}

class ProbMaker : public QWidget
{
    Q_OBJECT

public:
    explicit ProbMaker(QWidget *parent = 0);
    ~ProbMaker();

private:
    Ui::ProbMaker *ui;
};

#endif // PROBMAKER_H
