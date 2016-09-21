#include "answerboard.h"
#include "ui_answerboard.h"

namespace procon{
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
    this->field = procon::make_unique<procon::Field>(field);
    this->update();

    //add putid_list
    for(auto piece : this->field->getPieces()){
        if(piece.getId() != -1 && (piece.getId() != 0 && piece.getId() != 1)) putid_list.push_back(piece.getId());
    }
    if(this->field->getPiecesSize() >= 1) putid_left = this->field->getPiece(0).getId();
    if(this->field->getPiecesSize() >= 2) putid_right = this->field->getPiece(1).getId();
}

void AnswerBoard::setRawPicture(const cv::Mat& raw_pieces_pic, const std::vector<cv::Point>& pieces_pos)
{
    is_set_rawpic = true;
    cv::cvtColor(raw_pieces_pic, raw_pieces_pic, CV_RGB2BGR);
    this->pieces_pic = procon::make_unique<QImage>(raw_pieces_pic.data, raw_pieces_pic.cols, raw_pieces_pic.rows, raw_pieces_pic.step, QImage::Format_RGB888);
    *(this->pieces_pic) = QImage(raw_pieces_pic.data, raw_pieces_pic.cols, raw_pieces_pic.rows, raw_pieces_pic.step, QImage::Format_RGB888).copy();
    this->pieces_pos = procon::make_unique<std::vector<cv::Point>>(pieces_pos);
    this->update();
}

void AnswerBoard::setRandomColors(const std::vector<cv::Vec3b> &random_colors)
{
    this->random_colors = procon::make_unique<std::vector<cv::Vec3b>>(random_colors);
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

    auto drawPolygonInners = [&](polygon_t polygon,Space isLeftOrRight){
        int inners_num = polygon.inners().size();

        for(int inner = 0; inner < inners_num; inner++){

            int dot_num = polygon.inners().at(inner).size();
            QPointF* draw_point = new QPointF[dot_num];

            for(int dot = 0; dot < dot_num; dot++){
                draw_point[dot] = getPosition(QPointF(polygon.inners().at(inner).at(dot).x()/flame_size,polygon.inners().at(inner).at(dot).y()/flame_size),isLeftOrRight);
            }
            painter.drawPolygon(draw_point,dot_num);
            delete[] draw_point;
        }
    };

    static const QString color_background = "#d4c91f";
    static const QString color_piece      = "#0f5ca0";
    static const QString color_flame      = "#d0b98d";
    static const QString color_inner      = "#d4c91f";
    static const QString color_id         = "#ff33cc";
    static const QString color_arrow_left = "#ff0000";
    static const QString color_arrow_right= "#00ff00";
    static const QString color_corner_begin_id = "#00ffff";

    std::array<QPointF,50> field_pieces_pos;
    std::array<QPointF,50> rawpic_pieces_pos;

    //draw background
    painter.setBrush(QBrush(QColor(color_background)));
    painter.drawRect(0,0,this->width(),this->height());

    //draw field
    if(is_set_field){
        auto drawPiece = [&](procon::ExpandedPolygon piece){
            int piece_id = piece.getId();
            bool has_connector = piece_id == -1 ? false : is_set_rawpic ? true : false;
            //get polygon center pos
            point_t center = {0,0};
            boost::geometry::centroid(piece.getPolygon(), center);
            QPointF display_pos = getPosition(QPointF((center.x()/flame_size)-0.025, (center.y()/flame_size)+0.025), Space::LEFT);
            if(has_connector) field_pieces_pos.at(piece_id) = display_pos;

            //draw piece
            painter.setPen(QPen(Qt::black, 3));
            if(has_connector) painter.setBrush(QBrush(QColor(random_colors->at(piece_id)[2],random_colors->at(piece_id)[1],random_colors->at(piece_id)[0])));
            else painter.setBrush(QBrush(QColor(color_piece)));
            drawPolygon(piece.getPolygon(),Space::LEFT);
            //draw number
            if(is_set_rawpic){
                painter.setPen(QPen(QColor(color_id)));
                QFont font = painter.font();
                font.setPointSize(std::abs(getScale()/30));
                painter.setFont(font);
                painter.drawText(display_pos, (piece_id != -1) ? QString::number(piece_id) : QString::fromStdString(piece.makeMultiIdString()));
                //draw corner begin ID
                auto corner = piece.getPolygon().outer()[0];
                QPointF display_pos = getPosition(QPointF((corner.x()/flame_size)-0.0125, (corner.y()/flame_size)+0.0125), Space::LEFT);
                painter.setPen(QPen(QColor(color_corner_begin_id)));
                font.setPointSize(std::abs(getScale()/50));
                painter.setFont(font);
                painter.drawText(display_pos, "s"+QString::number(piece.getId()));
            }
        };

        //
        //int count = 0;
        //for(auto piece : field->getElementaryPieces()){
        //    displays.push_back(new SinglePolygonDisplay());
        //    displays[count]->setPolygon(piece,30,std::to_string(count));
        //    displays[count]->show();
        //    count++;
        //}
        //draw flame
        painter.setPen(QPen(Qt::black, 3));
        painter.setBrush(QBrush(QColor(color_flame)));
        drawPolygon(field->getFlame().getPolygon(),Space::LEFT);

        //draw flame inners
        painter.setBrush(QBrush(QColor(color_inner)));
        drawPolygonInners(field->getFlame().getPolygon(),Space::LEFT);

        //draw pieces
        for(auto piece : field->getPieces()){
            drawPiece(piece);
        }

        //draw flame-jointed pieces
        for(auto piece : field->getFlame().jointed_pieces){
            drawPiece(piece);
        }
    }

    //draw rawpic
    if(is_set_rawpic){
        //draw pic
        double rawpic_height_margin = (1-((double)pieces_pic->height()/(double)pieces_pic->width()))/2;
        painter.drawImage(QRectF(getPosition(QPointF(0,rawpic_height_margin),RIGHT),getPosition(QPointF(1,1-rawpic_height_margin),RIGHT)), *pieces_pic);
        //draw number
        int count = 0;
        for(cv::Point& pos : *pieces_pos){
            QPointF display_pos = getPosition(QPointF(((double)pos.x/(double)pieces_pic->width())-0.025,(rawpic_height_margin + ((double)pieces_pic->height()/(double)pieces_pic->width()) * ((double)pos.y/(double)pieces_pic->height()))+0.025), Space::RIGHT);
            rawpic_pieces_pos.at(count) = display_pos;

            painter.setPen(QPen(QColor(color_id)));
            QFont font = painter.font();
            font.setPointSize(std::abs(getScale()/30));
            painter.setFont(font);
            painter.drawText(display_pos, QString::number(count));
            count++;
        }
    }

    //draw arrow
    if(is_set_field && is_set_rawpic){
        if(putid_left != -1){
            painter.setPen(QPen(QColor(color_arrow_left), 5));
            painter.drawLine(field_pieces_pos.at(putid_left),rawpic_pieces_pos.at(putid_left));
        }
        if(putid_right != -1){
            painter.setPen(QPen(QColor(color_arrow_right), 5));
            painter.drawLine(field_pieces_pos.at(putid_right),rawpic_pieces_pos.at(putid_right));
        }
    }
}

void AnswerBoard::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
        case 16781616:
            if(putid_list.size() == 0){
                putid_left = -1;
            }else{
                putid_left = putid_list.back();
                putid_list.pop_back();
            }
            this->update();
            break;
        case 16777220:
            if(putid_list.size() == 0){
                putid_right = -1;
            }else{
                putid_right = putid_list.back();
                putid_list.pop_back();
            }
            this->update();
            break;
        default:
            std::cout<<"Press Key : "<<event->key()<<std::endl;
            break;
    }
}
