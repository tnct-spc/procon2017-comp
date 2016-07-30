#include "answerboard.h"
#include "ui_answerboard.h"

#include <QPainter>
#include <QPen>

AnswerBoard::AnswerBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnswerBoard)
{
    ui->setupUi(this);
}

AnswerBoard::~AnswerBoard()
{
    delete ui;
}

void AnswerBoard::setField(Field &field)
{
    this->field = field;
}

void AnswerBoard::paintEvent(QPaintEvent *)
{
    int height = this->height();
    int width = this->width();
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 3));

    auto drawLine = [&](double x1, double y1, double x2, double y2){
        static const int max = 30;
        static const int margin = 20;
        int y = height - margin;
        int x = height - margin;
        int y_margin = margin/2;
        int x_margin = (width-height)/2 + margin/2;
        painter.drawLine((x1*x/max)+x_margin, (y1*y/max)+y_margin, (x2*x/max)+x_margin, (y2*y/max)+y_margin);
    };

    auto drawPolygon = [&](std::vector<QPointF> points){
        int size = points.size();
        QPointF* draw_point = new QPointF[points.size()];
        static const int max = 30;
        static const int margin = 20;
        int y = height - margin;
        int x = height - margin;
        int y_margin = margin/2;
        int x_margin = (width-height)/2 + margin/2;
        for(int i=0;i<size;i++){
            draw_point[i].setX(((points.at(i).x())*x/max)+x_margin);
            draw_point[i].setY(((points.at(i).y())*y/max)+y_margin);
        }
        painter.drawPolygon(draw_point,size);
        delete[] draw_point;
    };

    //Field
    painter.setBrush(QBrush(QColor("#d4c91f")));
    painter.drawRect(0,0,width,height);

    //flame
    polygon_t raw_flame = field.getFlame().getPolygon();
    int flame_size = raw_flame.outer().size();
    std::vector<QPointF> points;
    for(int i=0;i<flame_size;i++){
        points.push_back(QPointF(raw_flame.outer()[i].x(),raw_flame.outer()[i].y()));
    }
    painter.setBrush(QBrush(QColor("#d0b98d")));
    drawPolygon(points);

    //piece
    std::vector<polygon_t> raw_pieces;
    int pieces_size = field.pieceSize();
    for(int i=0;i<pieces_size;i++){
        int piece_size = field.getPiece(i).getPolygon().outer().size()-1;
        std::vector<QPointF> points;
        for(int j=0;j<piece_size;j++){
            points.push_back(QPointF(field.getPiece(i).getPolygon().outer()[j].x(),field.getPiece(i).getPolygon().outer()[j].y()));
        }
        painter.setBrush(QBrush(QColor("#0f5ca0")));
        drawPolygon(points);
    }

}
