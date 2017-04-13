#include "answerboard.h"
#include "ui_answerboard.h"
#include "utilities.h"

//#define GOSA_CHECK_MODE
//#define HYOKA_MODE

std::unique_ptr<QImage> AnswerBoard::pieces_pic;
std::unique_ptr<std::vector<cv::Point>> AnswerBoard::pieces_pos;
std::unique_ptr<std::vector<cv::Vec3b>> AnswerBoard::random_colors;
bool AnswerBoard::is_set_rawpic = false;

AnswerBoard::AnswerBoard(int id, bool is_single_mode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnswerBoard),
    id(id),
    SINGLE_MODE(is_single_mode)
{
    ui->setupUi(this);

    connect(this,&AnswerBoard::clicked,this,&AnswerBoard::handleClickAction);
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

    //set putid_list
    putid_list.clear();
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
        putid_left = pieces.front().getId();
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

    //サポート矢印のためのピース位置保管変数
    std::array<QPointF,50> field_pieces_pos;
    std::array<QPointF,50> rawpic_pieces_pos;

    // Draw background
    painter.setBrush(QBrush(QColor(color_background)));
    painter.drawRect(0,0,this->width(),this->height());

    // Draw field
    if(is_set_field){
        auto drawPiece = [&](procon::ExpandedPolygon piece){
            int piece_id = piece.getId();
            bool has_connector = piece_id == -1 ? false : is_set_rawpic ? true : false;
            //get polygon center pos
            point_t center = {0,0};
            boost::geometry::centroid(piece.getPolygon(), center);
            QPointF display_pos = getPosition(QPointF((center.x()/frame_size)-0.025, (center.y()/frame_size)+0.025), Space::LEFT);
            QPointF inverse_display_pos = getPosition(QPointF((center.x()/frame_size)-0.035, (center.y()/frame_size)+0.035), Space::LEFT);
            if(has_connector) field_pieces_pos.at(piece_id) = display_pos;

            //draw piece
            painter.setPen(QPen(Qt::black, 0));
            if(has_connector) painter.setBrush(QBrush(QColor(random_colors->at(piece_id)[2],random_colors->at(piece_id)[1],random_colors->at(piece_id)[0])));
            else painter.setBrush(QBrush(QColor(color_piece)));
            drawPolygon(piece.getPolygon(),Space::LEFT);
            //draw number
            if(is_set_rawpic){
                //inverse color
                if(piece.is_inverse){
                    painter.setPen(QPen(QColor(color_id)));
                    QFont font = painter.font();
                    font.setPointSize(std::abs(getScale()/20));
                    painter.setFont(font);
                    painter.setPen(QPen(QColor("#ffffff")));
                    painter.drawText(inverse_display_pos, "●");
                }

                //draw piece id
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

        // Draw frame outer
        painter.setPen(QPen(Qt::black, 0));
        painter.setBrush(QBrush(QColor(color_frame)));
        drawPolygon(field->getFrame().getPolygon(),Space::LEFT);

        // Draw frame inners
        painter.setBrush(QBrush(QColor(color_inner)));
        drawPolygonInners(field->getFrame().getPolygon(),Space::LEFT);

        // Draw pieces(使わなかった)
        for(auto piece : field->getPieces()){
            drawPiece(piece);
        }

        // Draw pieces jointed frame
        for(procon::ExpandedPolygon piece : field->getFrame().getJointedPieces()){
#ifdef GOSA_CHECK_MODE
            piece.translatePolygon(30,0);
#endif
            drawPiece(piece);
        }

        // Draw Evaluation
        QPointF display_pos = getPosition(QPointF(0/frame_size,3/frame_size), Space::LEFT);
        painter.setPen(QPen(QColor(color_id)));
        QFont font = painter.font();
        font.setPointSize(std::abs(getScale()/(SINGLE_MODE?5:15)));
        painter.setFont(font);
        painter.drawText(display_pos, QString::number(field->getTotalEvaluation())+"     :     "+QString::number(field->getFrame().getJointedPieces().size())+"/"+QString::number(field->getElementaryPieces().size()));
#ifdef HYOKA_MODE
        // Draw each Evaluation
        display_pos = getPosition(QPointF(0/frame_size,6/frame_size), Space::LEFT);
        painter.setPen(QPen(QColor(color_id)));
        font = painter.font();
        font.setPointSize(std::abs(getScale()/(SINGLE_MODE?10:30)));
        painter.setFont(font);
        painter.drawText(display_pos, QString::number(field->evaluation_sum) + "=" + QString::number(field->evaluation_normal) + "+" + QString::number(field->evaluation_angle) + "+" + QString::number(field->evaluation_length) + "+" + QString::number(field->evaluation_history) + "+" + QString::number(field->evaluation_frame));
#endif
    }

    // Draw rawpic
#ifdef GOSA_CHECK_MODE
    if(false){
#else
    if(!SINGLE_MODE && is_set_rawpic){
#endif
        // Draw pic
        double rawpic_height_margin = (1-((double)pieces_pic->height()/(double)pieces_pic->width()))/2;
        painter.drawImage(QRectF(getPosition(QPointF(0,rawpic_height_margin),RIGHT),getPosition(QPointF(1,1-rawpic_height_margin),RIGHT)), *pieces_pic);

        // Draw pieces numbers
        std::vector<bool> is_put;
        int piece_size = field->getElementaryPieces().size();
        for(int i=0;i<piece_size;i++){
            is_put.push_back(false);
        }
        for(auto piece : field->getFrame().getJointedPieces()){
            is_put.at(piece.getId()) = true;
        }
        for(unsigned int piece_count = 0; piece_count < pieces_pos->size(); ++piece_count){
            QPointF display_pos = getPosition(QPointF(((double)pieces_pos->at(piece_count).x/(double)pieces_pic->width())-0.025,(rawpic_height_margin + ((double)pieces_pic->height()/(double)pieces_pic->width()) * ((double)pieces_pos->at(piece_count).y/(double)pieces_pic->height()))+0.025), Space::RIGHT);
            QPointF inverse_display_pos = getPosition(QPointF(((double)pieces_pos->at(piece_count).x/(double)pieces_pic->width())-0.035,(rawpic_height_margin + ((double)pieces_pic->height()/(double)pieces_pic->width()) * ((double)pieces_pos->at(piece_count).y/(double)pieces_pic->height()))+0.035), Space::RIGHT);
            rawpic_pieces_pos.at(piece_count) = display_pos;

            // Draw circle if polygon inversed
            bool is_inverse = false;
            for(auto piece : field->getFrame().getJointedPieces()){
                if(static_cast<int>(piece_count) == piece.getId() && piece.is_inverse){
                    is_inverse = true;
                    break;
                }
            }
            if(is_inverse){
                painter.setPen(QPen(QColor(color_id)));
                QFont font = painter.font();
                font.setPointSize(std::abs(getScale()/20));
                painter.setFont(font);
                painter.setPen(QPen(QColor("#ffffff")));
                painter.drawText(inverse_display_pos, "●");
            }

            // Draw number
            painter.setPen(QPen(QColor(color_id)));
            QFont font = painter.font();
            font.setPointSize(std::abs(getScale()/30));
            painter.setFont(font);
            painter.drawText(display_pos, QString::number(piece_count));

            // Draw "@" if piece hadn't put
            if(is_put.at(piece_count) == false){
                painter.setPen(QPen(QColor(color_id)));
                QFont font = painter.font();
                font.setPointSize(std::abs(getScale()/20));
                painter.setFont(font);
                painter.setPen(QPen(QColor("#0000ff")));
                painter.drawText(inverse_display_pos, "●");
                painter.setPen(QPen(QColor("#00ff00")));
                painter.drawText(inverse_display_pos, "@");
            }
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
        case 16781616:// caps lock
            //左プレイヤーが置くピースを切り替える
            if(putid_list.size() == 0){
                putid_left = -1;
            }else{
                putid_left = putid_list.front();
                putid_list.erase(putid_list.begin());
            }
            this->update();
            break;
        case 16777220:// enter
            //右プレイヤーが置くピースを切り替える
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

void AnswerBoard::handleClickAction(QMouseEvent *event)
{
    int mousebutton = event->buttons();

    if(mousebutton == Qt::RightButton){
        if(SINGLE_MODE){
            ans_board = new AnswerBoard();
            ans_board->setField(*field);
            ans_board->showMaximized();
        }
    }else if(mousebutton == Qt::LeftButton){
        if(SINGLE_MODE){
            emit clicked_with_id(this->id);
        }
    }
}
