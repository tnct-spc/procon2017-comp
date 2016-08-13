#include "answerboard.h"
#include "ui_answerboard.h"

namespace std {
    template<typename T, typename ...Args>
    std::unique_ptr<T> make_unique( Args&& ...args )
    {
        return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
    }
}

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

void AnswerBoard::setField(const procon::Field &field)
{
    is_set_field = true;
    this->field = std::make_unique<procon::Field>(field);
    this->update();
}

void AnswerBoard::setRawPicture(const cv::Mat& raw_pieces_pic, const std::vector<cv::Point>& pieces_pos)
{
    is_set_rawpic = true;
    cv::cvtColor(raw_pieces_pic, raw_pieces_pic, CV_RGB2BGR);
    this->pieces_pic = std::make_unique<QImage>(raw_pieces_pic.data, raw_pieces_pic.cols, raw_pieces_pic.rows, raw_pieces_pic.step, QImage::Format_RGB888);
    *(this->pieces_pic) = QImage(raw_pieces_pic.data, raw_pieces_pic.cols, raw_pieces_pic.rows, raw_pieces_pic.step, QImage::Format_RGB888).copy();
    this->pieces_pos = std::make_unique<std::vector<cv::Point>>(pieces_pos);
    this->update();
}

QPointF AnswerBoard::getPosition(QPointF point_percent, Space space){
    int height = this->height();
    int width  = this->width();
    int height_size = height - (top_margin + bottom_margin);
    int width_size  = ( width - (left_margin + right_margin) )/2;
    int image_size = height_size < width_size ? height_size : width_size;
    int y_padding  = height_size < width_size ? 0 : (height_size-image_size)/2;
    int x_padding  = height_size < width_size ? (width_size-image_size)/2 : 0;
    return QPointF((point_percent.x()*image_size) + x_padding + left_margin + (space == LEFT || space == OVERALL ? 0 : width_size),
                   (point_percent.y()*image_size) + y_padding +  top_margin);
}

double AnswerBoard::getScale(){
    int height = this->height();
    int width  = this->width();
    int height_size = height - (top_margin + bottom_margin);
    int width_size  = ( width - (left_margin + right_margin) )/2;
    int image_size = height_size < width_size ? height_size : width_size;
    return image_size;
}

void AnswerBoard::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    auto drawPolygon = [&](polygon_t polygon, Space isLeftOrRight){
        int dot_num = polygon.outer().size();
        QPointF* draw_point = new QPointF[dot_num];
        for(int i=0;i<dot_num;i++){
            draw_point[i] = getPosition(QPointF(polygon.outer()[i].x()/flame_size,polygon.outer()[i].y()/flame_size), isLeftOrRight);
        }
        painter.drawPolygon(draw_point,dot_num);
        delete[] draw_point;
    };

    static const QString color_background = "#d4c91f";
    static const QString color_piece      = "#0f5ca0";
    static const QString color_flame      = "#d0b98d";
    static const QString color_id         = "#ff33cc";

    //draw background
    painter.setBrush(QBrush(QColor(color_background)));
    painter.drawRect(0,0,this->width(),this->height());

    if(is_set_field){
        //draw flame
        painter.setPen(QPen(Qt::black, 3));
        painter.setBrush(QBrush(QColor(color_flame)));
        drawPolygon(field->getFlame().getPolygon(),Space::LEFT);

        //draw pieces
        int pieces_size = field->getPiecesSize();
        for(int i=0;i<pieces_size;i++){
            //get polygon center pos
            point_t center;
            boost::geometry::centroid(field->getPiece(i).getPolygon(), center);
            //draw piece
            painter.setPen(QPen(Qt::black, 3));
            painter.setBrush(QBrush(QColor(color_piece)));
            drawPolygon(field->getPiece(i).getPolygon(),Space::LEFT);
            //draw number
            painter.setPen(QPen(QColor(color_id)));
            QFont font = painter.font();
            font.setPointSize(std::abs(getScale()/15));
            painter.setFont(font);
            painter.drawText(getPosition(QPointF(center.x()/flame_size, center.y()/flame_size), Space::LEFT), QString::number(field->getPiece(i).getId()));
        }
    }
    if(is_set_rawpic){
        //draw pic
        double rawpic_height_margin = (1-((double)pieces_pic->height()/(double)pieces_pic->width()))/2;
        painter.drawImage(QRectF(getPosition(QPointF(0,rawpic_height_margin),RIGHT),getPosition(QPointF(1,1-rawpic_height_margin),RIGHT)), *pieces_pic);
        //draw number
        int count = 0;
        for(cv::Point& pos : *pieces_pos){
            painter.setPen(QPen(QColor(color_id)));
            QFont font = painter.font();
            font.setPointSize(std::abs(getScale()/15));
            painter.setFont(font);
            painter.drawText(getPosition(QPointF((double)pos.x/(double)pieces_pic->width(),rawpic_height_margin + ((double)pieces_pic->height()/(double)pieces_pic->width()) * ((double)pos.y/(double)pieces_pic->height())), Space::RIGHT), QString::number(count));
            count++;
        }
    }
}
