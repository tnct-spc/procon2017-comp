#include "answerboard.h"
#include "ui_answerboard.h"
#include "utilities.h"

std::unique_ptr<QImage> AnswerBoard::pieces_pic;
std::unique_ptr<std::vector<cv::Point>> AnswerBoard::pieces_pos;
std::unique_ptr<std::vector<cv::Vec3b>> AnswerBoard::random_colors;
bool AnswerBoard::is_set_rawpic = false;

AnswerBoard::AnswerBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnswerBoard)
{
    ui->setupUi(this);

    connect(this,&AnswerBoard::clicked,this,&AnswerBoard::printBigWindow);
}

AnswerBoard::~AnswerBoard()
{
    delete ui;
}

void AnswerBoard::setField(const procon::Field &field)
{
    is_set_field = true;
    this->field = Utilities::make_unique<procon::Field>(field);
    this->update();

    print_field = field;

    //add putid_list
    std::vector<procon::ExpandedPolygon> pieces = this->field->getFrame().getJointedPieces();
    std::sort(pieces.begin(),pieces.end(),[](procon::ExpandedPolygon const& rhs, procon::ExpandedPolygon const& lhs)->bool{
        const point_t r_center = bg::return_centroid<point_t>(rhs.getPolygon());
        const point_t l_center = bg::return_centroid<point_t>(lhs.getPolygon());
        return r_center.y() == l_center.y() ? r_center.x() < l_center.x() : r_center.y() < l_center.y();
    });
    for(auto piece : pieces){
        if(piece.getId() != -1) putid_list.push_back(piece.getId());
    }
    int jointed_pieces_size = pieces.size();
    if(jointed_pieces_size >= 1){
        putid_list.erase(putid_list.begin());
        putid_left = pieces.at(0).getId();
    }
    if(jointed_pieces_size >= 2){
        putid_list.pop_back();
        putid_right = pieces.back().getId();
    }
}

void AnswerBoard::setRawPicture(const cv::Mat& raw_pieces_pic, const std::vector<cv::Point>& pieces_pos_)
{
    is_set_rawpic = true;
    cv::cvtColor(raw_pieces_pic, raw_pieces_pic, CV_RGB2BGR);
    pieces_pic = Utilities::make_unique<QImage>(raw_pieces_pic.data, raw_pieces_pic.cols, raw_pieces_pic.rows, raw_pieces_pic.step, QImage::Format_RGB888);
    *(pieces_pic) = QImage(raw_pieces_pic.data, raw_pieces_pic.cols, raw_pieces_pic.rows, raw_pieces_pic.step, QImage::Format_RGB888).copy();
    pieces_pos = Utilities::make_unique<std::vector<cv::Point>>(pieces_pos_);
}

void AnswerBoard::setRandomColors(const std::vector<cv::Vec3b> &random_colors_)
{
    random_colors = Utilities::make_unique<std::vector<cv::Vec3b>>(random_colors_);
}

QPointF AnswerBoard::getPosition(QPointF point_percent, Space space){
    int height = this->height();
    int width  = this->width();
    int height_size = height - (SINGLE_MODE?0:(top_margin + bottom_margin));
    int width_size  = ( width - (SINGLE_MODE?0:(left_margin + right_margin)) ) / (SINGLE_MODE? 1:2);
    int image_size = height_size < width_size ? height_size : width_size;
    int y_padding  = height_size < width_size ? 0 : (height_size-image_size)/2;
    int x_padding  = height_size < width_size ? (width_size-image_size)/2 : 0;
    return QPointF((point_percent.x()*image_size) + x_padding + (SINGLE_MODE?0:left_margin) + (SINGLE_MODE? 0 : space == LEFT ? 0:space == OVERALL ? (width_size/2) : width_size),
                   (point_percent.y()*image_size) + y_padding +  (SINGLE_MODE?0:top_margin));
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
            draw_point[i] = getPosition(QPointF(polygon.outer()[i].x()/frame_size,polygon.outer()[i].y()/frame_size), isLeftOrRight);
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
                draw_point[dot] = getPosition(QPointF(polygon.inners().at(inner).at(dot).x()/frame_size,polygon.inners().at(inner).at(dot).y()/frame_size),isLeftOrRight);
            }
            painter.drawPolygon(draw_point,dot_num);
            delete[] draw_point;
        }
    };

    const QString color_background = SINGLE_MODE? "#E5E6DB":"#d4c91f";
    const QString color_piece      = "#0f5ca0";
    const QString color_frame      = SINGLE_MODE? "#7ACACB":"#d0b98d";
    const QString color_inner      = color_background;
    const QString color_id         = "#ff33cc";
    const QString color_arrow_left = "#ff0000";
    const QString color_arrow_right= "#00ff00";
    const QString color_corner_begin_id = "#00ffff";




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
            QPointF display_pos = getPosition(QPointF((center.x()/frame_size)-0.025, (center.y()/frame_size)+0.025), Space::LEFT);
            if(has_connector) field_pieces_pos.at(piece_id) = display_pos;

            //draw piece
            painter.setPen(QPen(Qt::black, 0));
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
                QPointF display_pos = getPosition(QPointF((corner.x()/frame_size)-0.0125, (corner.y()/frame_size)+0.0125), Space::LEFT);
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
        //    displays[count]->resetPolygonForce(piece,30,std::to_string(count));
        //    displays[count]->show();
        //    count++;
        //}
        //draw frame
        painter.setPen(QPen(Qt::black, 0));
        painter.setBrush(QBrush(QColor(color_frame)));
        drawPolygon(field->getFrame().getPolygon(),Space::LEFT);

        //draw frame inners
        painter.setBrush(QBrush(QColor(color_inner)));
        drawPolygonInners(field->getFrame().getPolygon(),Space::LEFT);

        //draw pieces
        for(auto piece : field->getPieces()){
            drawPiece(piece);
        }

        //draw frame-jointed pieces
        for(auto piece : field->getFrame().getJointedPieces()){
            drawPiece(piece);
        }

        //draw Eva
        QPointF display_pos = getPosition(QPointF(0/frame_size,3/frame_size), Space::LEFT);
        painter.setPen(QPen(QColor(color_id)));
        QFont font = painter.font();
        font.setPointSize(std::abs(getScale()/(SINGLE_MODE?5:15)));
        painter.setFont(font);
        painter.drawText(display_pos, QString::number(field->evaluation));

        //draw frame-jointed pieces number
        const int max_len = 10;
        int cnt = 0;
        for(auto piece : field->getFrame().getJointedPieces()){
            QPointF display_pos = getPosition(QPointF((27+3*(cnt/max_len))/frame_size,(3*(cnt%max_len))/frame_size), Space::LEFT);
            painter.drawText(display_pos, QString::number(piece.getId()));
            cnt++;
        }

    }

    //draw rawpic
    if(!SINGLE_MODE && is_set_rawpic){
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
    if(!SINGLE_MODE && is_set_field && is_set_rawpic){
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
                putid_left = putid_list.front();
                putid_list.erase(putid_list.begin());
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
            //std::cout<<"Press Key : "<<event->key()<<std::endl;
            break;
    }
}

void AnswerBoard::mousePressEvent(QMouseEvent *event)
{
    emit clicked(event);
}
#include <QMouseEvent>
void AnswerBoard::printBigWindow(QMouseEvent *event)
{
    //emit clicked_with_id(id);

    //AnswerBoard ans;

    int mousebutton = event->buttons();

    if(SINGLE_MODE){

        if(mousebutton == Qt::RightButton){
                ans_board = new AnswerBoard();
                ans_board->setField(print_field);
                ans_board->showMaximized();
        }else if(mousebutton == Qt::LeftButton){

            emit clicked_with_id(id);

        }
    }



    //sleep(2000);
}
