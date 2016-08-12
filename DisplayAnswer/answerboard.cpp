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
    QPainter painter(this);

    enum Space
    {
        LEFT    = 0,
        RIGHT   = 1,
        OVERALL = 2
    };

    int height = this->height();
    int width  = this->width();
    int size   = height < width ? height : width;
    int cutted_size = height < (width/2) ? height : (width/2);

    auto getPosition = [&](QPointF point, Space space) -> QPointF{
        static const int max = 30;
        static const int top_margin    = 10;
        static const int bottom_margin = 10;
        static const int left_margin   = 10;
        static const int right_margin  = 10;
        int height_size = height - (top_margin + bottom_margin);
        int width_size  = ( width - (left_margin + right_margin) )/2;
        int image_size = height_size < width_size ? height_size : width_size;
        int y_padding  = height_size < width_size ? 0 : (height_size-image_size)/2;
        int x_padding  = height_size < width_size ? (width_size-image_size)/2 : 0;
        return QPointF(((point.x()/max)*image_size) + x_padding + left_margin + (space == LEFT || space == OVERALL ? 0 : width_size),
                       ((point.y()/max)*image_size) + y_padding +  top_margin);
    };

    auto drawPolygon = [&](polygon_t polygon, Space isLeftOrRight){
        int dot_num = polygon.outer().size();
        QPointF* draw_point = new QPointF[dot_num];
        for(int i=0;i<dot_num;i++){
            draw_point[i] = getPosition(QPointF(polygon.outer()[i].x(),polygon.outer()[i].y()), isLeftOrRight);
        }
        painter.drawPolygon(draw_point,dot_num);
        delete[] draw_point;
    };

    //draw background
    painter.setBrush(QBrush(QColor("#d4c91f")));
    painter.drawRect(0,0,width,height);

    //draw flame
    painter.setPen(QPen(Qt::black, 3));
    painter.setBrush(QBrush(QColor("#d0b98d")));
    drawPolygon(field.getFlame().getPolygon(),Space::LEFT);

    //draw pieces
    int pieces_size = field.getPiecesSize();
    for(int i=0;i<pieces_size;i++){
        //get polygon center pos
        int piece_size = field.getPiece(i).getPolygon().outer().size()-1;
        double center_x = 0.0;
        double center_y = 0.0;
        for(int j=0;j<piece_size;j++){
            center_x += field.getPiece(i).getPolygon().outer()[j].x();
            center_y += field.getPiece(i).getPolygon().outer()[j].y();
        }
        center_x /= piece_size;
        center_y /= piece_size;
        //draw piece
        painter.setPen(QPen(Qt::black, 3));
        painter.setBrush(QBrush(QColor("#0f5ca0")));
        drawPolygon(field.getPiece(i).getPolygon(),Space::LEFT);
        //draw number
        painter.setPen(QPen(QColor("#ff33cc")));
        QFont font = painter.font();
        font.setPointSize(std::abs(cutted_size/15));
        painter.setFont(font);
        painter.drawText(getPosition(QPointF(center_x, center_y), Space::LEFT), QString::number(i));
    }

}
