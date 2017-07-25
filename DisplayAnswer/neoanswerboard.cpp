#include "neoanswerboard.h"
#include "ui_neoanswerboard.h"
#include "utilities.h"

#include <QPainter>
#include <QPaintEvent>
#include <QColor>
#include <iostream>
#include <random>
#include <array>
#include <opencv2/core/core.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/strategies/transform.hpp>
#include <boost/geometry/strategies/transform/matrix_transformers.hpp>
#include <boost/geometry/algorithms/disjoint.hpp>
#include "../Polygon/neoexpandedpolygon.h"

NeoAnswerBoard::NeoAnswerBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NeoAnswerBoard)
{
    ui->setupUi(this);
}

NeoAnswerBoard::~NeoAnswerBoard()
{
    delete ui;
}
QRgb NeoAnswerBoard::NeosetRandomColors()
{
    std::random_device rnd;
    std::uniform_int_distribution<int> rand255;
    QRgb randomcolors = (rand255(rnd), rand255(rnd), rand255(rnd), 255);
    return randomcolors;

}

void NeoAnswerBoard::beforePolygon()
{
    //format
    //const int picture_width;
    //const int picture_height;
    const int window_width = this->width();
    const int window_height = this->height();
    const int app_height = window_height / 2;
    //const int top_bottom_margin;
}

void NeoAnswerBoard::paintEvent(QPaintEvent *event)
{
    const QString back_ground_color = "#00FFFF";

    const int window_width = this->width();
    const int window_height = this->height();

    // 101 x 65
    const int grid_row = 65;
    const int grid_col = 101;
    const int grid_margin = 1;
    QPainter painter(this);

    //draw background
    painter.setBrush(QBrush(QColor(back_ground_color)));
    painter.drawRect(QRect(0,0,window_width,window_height/2));
    painter.setPen(QPen(QBrush(Qt::black),0.1));


    //draw grid
    auto drawGrid = [&]{
        const int splitedheight = window_height/2;
        grid_size =
                window_width <= splitedheight
                ? window_width / (grid_col + grid_margin)
                : splitedheight / (grid_row + grid_margin);
        const int top_bottom_margin = (splitedheight - grid_size * grid_row) / 2;
        const int left_right_margin = (window_width - grid_size * grid_col) / 2;
        for (int current_col = 0; current_col < grid_col + 1; ++current_col) {
            int x = current_col * this->grid_size + left_right_margin;
            painter.drawLine(x,top_bottom_margin,x,splitedheight - top_bottom_margin);
        }

        for (int current_row = 0; current_row < grid_row + 1; ++current_row) {
            int y = current_row * grid_size + top_bottom_margin;
            painter.drawLine(left_right_margin,y,window_width - left_right_margin,y);
        }
    };

    auto testColors = [&]{
        QRgb hoge = NeosetRandomColors();
        painter.setBrush(QBrush(QColor(hoge)));
        static const QPointF points[4] = {
            QPointF(10.0, 80.0),
            QPointF(20.0, 10.0),
            QPointF(80.0, 30.0),
            QPointF(90.0, 70.0)
        };
        painter.drawPolygon(points, 4);
        QRgb hooo = NeosetRandomColors();
        painter.setBrush(QBrush(QColor(hooo)));
        static const QPointF pointss[4] = {
            QPointF(100.0, 400.0),
            QPointF(200.0, 100.0),
            QPointF(800.0, 300.0),
            QPointF(900.0, 700.0)
        };
        painter.drawPolygon(pointss, 4);
    };
    drawGrid();
    testColors();
}
