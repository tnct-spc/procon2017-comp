#ifndef PROBMAKER_H
#define PROBMAKER_H

#include <QWidget>
#include "neoexpandedpolygon.h"

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
    void run();
    std::vector<std::pair<point_i,point_i>> lines;

protected:
    void paintEvent(QPaintEvent *);
};

#endif // PROBMAKER_H
