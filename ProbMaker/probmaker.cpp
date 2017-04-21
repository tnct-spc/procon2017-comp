#include "probmaker.h"
#include "ui_probmaker.h"

#include <iostream>
#include <random>

ProbMaker::ProbMaker(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProbMaker)
{
    run();
    ui->setupUi(this);
}

ProbMaker::~ProbMaker()
{
    delete ui;
}

void ProbMaker::run()
{
    std::cout << "run" << std::endl;
    make_frame();
}

void ProbMaker::make_frame()
{
    // フレームの角数を取得
    // 各角の座標をランダムで取得
    // 各同士が近すぎないか確認

    std::vector<Point> frame;
    int x,y;
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_int_distribution<int> horizontal_limit(0,100);
    std::uniform_int_distribution<int> vartical_limit(0,64);

    for (int i=0; i<4; i++) {
        x = horizontal_limit(mt);
        y = vartical_limit(mt);
        frame.push_back(Point(x,y));
    }

    for (int i=0; i<4; i++) {
        std::cout << "(" << frame[i].x << "," << frame[i].y << ")" << std::endl;
    }
}
