#ifndef PROBMAKER_H
#define PROBMAKER_H

// #include "expandedpolygon.h"


#include <QMainWindow>


namespace Ui {
class ProbMaker;
}

class Point {

public:

    // 点の座標上の位置
    int x;
    int y;

    // コンストラクタ
    Point() {
        x = 0;
        y = 0;
    }

    Point(int x_, int y_) {
        x = x_;
        y = y_;
    }
};

class ProbMaker : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProbMaker(QWidget *parent = 0);
    ~ProbMaker();
    void run();

private:
    Ui::ProbMaker *ui;
    void make_frame(); // フレームを作る
    void make_pieces(); // ピースを作る
};

#endif // PROBMAKER_H
