#include "neoanswerboard.h"
#include "ui_neoanswerboard.h"

#include "field.h"
#include "expandedpolygon.h"


#include <QPainter>
#include <QPaintEvent>

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

void NeoAnswerBoard::paintEvent(QPaintEvent *event)
{
    const QString back_ground_color = "#00FFFF";
    const int window_width = this->width();
    const int window_height = this->height();

    // 101 x 65
    const int grid_row = 65;
    const int grid_col = 101;
    const int grid_margin = 1;
    const int splitedheight = window_height/2;
    grid_size =
            window_width <= splitedheight
            ? window_width / (grid_col + grid_margin)
            : splitedheight / (grid_row + grid_margin);
    top_bottom_margin = (splitedheight - grid_size * grid_row) / 2;
    left_right_margin = (window_width - grid_size * grid_col) / 2;

    top_margin = grid_size * grid_margin;
    QPainter painter(this);

    //draw background
    painter.setBrush(QBrush(QColor(back_ground_color)));
    painter.drawRect(QRect(0,0,window_width,window_height/2));
    painter.setPen(QPen(QBrush(Qt::black),0.5));


    //draw grid
    auto drawGrid = [&]{
        painter.setPen(QPen(QBrush(Qt::black),0.1));
        for (int current_col = 0; current_col < grid_col + 1; ++current_col) {
            int x = current_col * grid_size + left_right_margin;
            painter.drawLine(x,top_bottom_margin,x,splitedheight - top_bottom_margin);
        }
        for (int current_row = 0; current_row < grid_row + 1; ++current_row) {
            int y = current_row * grid_size + top_bottom_margin;
            painter.drawLine(left_right_margin,y,window_width - left_right_margin,y);
        }
    };
    auto setField = [&]{
        procon::ExpandedPolygon polygon;
        framepolygon.outer().push_back(point_t(5,5));
        framepolygon.outer().push_back(point_t(85,7));
        framepolygon.outer().push_back(point_t(75,48));
        framepolygon.outer().push_back(point_t(18,57));
        framepolygon.outer().push_back(point_t(5,5));
        polygon.resetPolygonForce(framepolygon);
        field.setFrame(polygon);
    };
    auto drawFrame = [&]{
        painter.setBrush(QBrush(QColor(236,182,138))); //set color
        painter.drawRect(QRect(left_right_margin,top_bottom_margin,grid_col*grid_size,grid_row*grid_size));
        QPointF points[4];
        for(int tes = 0;tes < 4; tes++){
            points[tes] = getPosition(field.getFrame().getPolygon().outer().at(tes));
        }
        painter.setBrush(QBrush(QColor(back_ground_color)));
        painter.drawPolygon(points,4);
    };
    setField();
    drawFrame();
    drawGrid();
}
QPointF NeoAnswerBoard::getPosition(point_t point){
    return QPointF(left_right_margin + point.x() * grid_size, top_bottom_margin + point.y() * grid_size);
}
