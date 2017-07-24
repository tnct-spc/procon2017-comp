#include "neoanswerboard.h"
#include "ui_neoanswerboard.h"

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
    const int grid_margin = 2;

    QPainter painter(this);

    //draw background
    painter.setBrush(QBrush(QColor(back_ground_color)));
    painter.drawRect(QRect(0,0,window_width,window_height));



    //draw grid
    auto drawGrid = [&]{
        grid_size =
                window_width <= window_height
                ? window_width / (grid_col + grid_margin)
                : window_height / (grid_row + grid_margin);
        top_buttom_margin = (window_height - grid_size * grid_row) / 2;
        left_right_margin = (window_width - grid_size * grid_col) / 2;

        for (int current_col = 0; current_col < grid_col + 1; ++current_col) {
            int x = current_col * grid_size + left_right_margin;
            painter.drawLine(x,top_buttom_margin,x,window_height - top_buttom_margin);
        }
        for (int current_row = 0; current_row < grid_row + 1; ++current_row) {
            int y = current_row * grid_size + top_buttom_margin;
            painter.drawLine(left_right_margin,y,window_width - left_right_margin,y);
        }
    };
    auto setField = [&]{
        framepolygon.outer().push_back(point_i(5,5));
        framepolygon.outer().push_back(point_i(85,7));
        framepolygon.outer().push_back(point_i(75,48));
        framepolygon.outer().push_back(point_i(18,57));
        framepolygon.outer().push_back(point_i(5,5));
    };
    auto drawFrame = [&]{
        painter.setBrush(QBrush(QColor(QString("#00FF00")))); //set color
        QPointF points[4];
        for(int tes = 0;tes < 4; tes++){
            points[tes] = getPosition(framepolygon.outer().at(tes));
        }
        painter.drawPolygon(points,4);
    };
    setField();
    drawGrid();
    drawFrame();
}
QPointF NeoAnswerBoard::getPosition(point_i point){
    return QPointF(point.x(),point.y());
}
