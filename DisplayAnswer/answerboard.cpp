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

    auto drawLine = [&](int x1, int y1, int x2, int y2){
        static const int max = 30;
        static const int margin = 20;
        int y = height - margin;
        int x = height - margin;
        int y_margin = margin/2;
        int x_margin = (width-height)/2 + margin/2;
        painter.drawLine((x1*x/max)+x_margin, (y1*y/max)+y_margin, (x2*x/max)+x_margin, (y2*y/max)+y_margin);
    };

    //drawField
    polygon_t flame = field.getFlame();
    std::vector<polygon_t> pieces;
    pieces.push_back(field.getPiece(0));
    //flame
    int flame_size = flame.outer().size();
    for(int i=0;i<flame_size;i++){
        drawLine(flame.outer()[i].x(), flame.outer()[i].y(), flame.outer()[(i+1)%flame_size].x(), flame.outer()[(i+1)%flame_size].y());
    }
    //piece
    int pieces_size = 1;
    for(int i=0;i<pieces_size;i++){
        int piece_size = pieces[i].outer().size();
        for(int j=0;j<piece_size;j++){
            drawLine(pieces[i].outer()[j].x(), pieces[i].outer()[j].y(), pieces[i].outer()[(j+1)%flame_size].x(), pieces[i].outer()[(j+1)%flame_size].y());
        }
    }

}
