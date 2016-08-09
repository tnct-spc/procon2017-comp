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

void AnswerBoard::setField(procon::Field &field)
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

    //drawField
    polygon_t raw_flame = field.getFlame().getPolygon();
    //flame
    int flame_size = raw_flame.outer().size();
    for(int i=0;i<flame_size;i++){
        drawLine(raw_flame.outer()[i].x(), raw_flame.outer()[i].y(), raw_flame.outer()[(i+1)%flame_size].x(), raw_flame.outer()[(i+1)%flame_size].y());
    }
    //piece
    std::vector<polygon_t> raw_pieces;
    int pieces_size = field.getPiecesSize();
    for(int i=0;i<pieces_size;i++){
        raw_pieces.push_back(field.getPiece(i).getPolygon());
        int piece_size = raw_pieces[i].outer().size()-1;
        for(int j=0;j<piece_size;j++){
            drawLine(raw_pieces[i].outer()[j].x(), raw_pieces[i].outer()[j].y(), raw_pieces[i].outer()[(j+1)%flame_size].x(), raw_pieces[i].outer()[(j+1)%flame_size].y());
        }
    }

}
