#include "neoanswerboard.h":
#include "ui_neoanswerboard.h"

NeoAnswerBoard::NeoAnswerBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NeoAnswerBoard)
{
    ui->setupUi(this);
    firstField();
    setRandomColors(20);
}

NeoAnswerBoard::~NeoAnswerBoard()
{
    delete ui;
}
void NeoAnswerBoard::setRandomColors(int threshold)
{
    std::random_device rnd;
    std::uniform_int_distribution<int> rand255(0, 255);
    cv::Vec3b up_colors;
    cv::Vec3b down_colors;
    colors.resize(field.getPieces().size());
    cv::Vec3b frame_color = {236,182,138};
    int flag;
    for(int i = 0; i < field.getPieces().size(); ++i){
        flag = 1;
        while(flag){
            flag = 0;
            colors[i] = {rand255(rnd), rand255(rnd), rand255(rnd)};
            for(int j = 0; j < i; j++){
                up_colors = {colors[i][0] + threshold, colors[i][1] + threshold, colors[i][2] + threshold};
                down_colors = {colors[i][0] - threshold, colors[i][1] - threshold, colors[i][2] - threshold};
                if(colors[i][0] < up_colors[0] && colors[i][0] > down_colors[0] ){
                    if(colors[i][1] < up_colors[1] && colors[i][1] > down_colors[1]){
                        if(colors[i][2] < up_colors[2] && colors[i][2] > down_colors[2]){
                            flag = 1;
                            break;
                        }
                    }
                }else if(colors[i] == frame_color){
                    flag = 1;
                    break;
                }
            }
        }
    }
}

void NeoAnswerBoard::paintEvent(QPaintEvent *event)
{
    const QString up_back_ground_color = "#00FFFF";
    const QString down_back_ground_color = "#66CDAA";
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
    down_up_y = splitedheight + top_bottom_margin;

    QPainter painter(this);

    //draw background
    painter.setBrush(QBrush(QColor(up_back_ground_color)));
    painter.drawRect(QRect(0,0,window_width,window_height/2));
    painter.setBrush(QBrush(QColor(down_back_ground_color)));
    painter.drawRect(QRect(0, splitedheight, window_width, window_height));


    //draw grid
    auto drawGrid = [&]{
        painter.setPen(QPen(QBrush(Qt::black),0.1));
        for (int current_col = 0; current_col < grid_col + 1; ++current_col) {
            int x = current_col * this->grid_size + left_right_margin;
            painter.drawLine(x,top_bottom_margin,x,splitedheight - top_bottom_margin);
        }
        for (int current_row = 0; current_row < grid_row + 1; ++current_row) {
            int y = current_row * grid_size + top_bottom_margin;
            painter.drawLine(left_right_margin,y,window_width - left_right_margin,y);
        }
    };

    //draw frame
    auto drawFrame = [&]{
        painter.setBrush(QBrush(QColor(236,182,138, 200))); //frame color
        painter.setPen(QPen(QBrush(Qt::black),grid_size*0.1));
        painter.drawRect(QRect(left_right_margin,top_bottom_margin,grid_col*grid_size,grid_row*grid_size));
//        int pcount = field.getFrame().getSize();
//        QPointF points[pcount];
//        for(int tes = 0;tes < pcount; tes++){
//            points[tes] = getPosition(field.getFrame().getPolygon().outer().at(tes));
//        }
//        painter.setBrush(QBrush(QColor(up_back_ground_color)));
//        painter.drawPolygon(points,pcount);
        std::vector<QPointF> frame_points;
        for(auto point : field.getFrame().getPolygon().outer()){
            frame_points.push_back(getPosition(point));
        }
        painter.setBrush(QBrush(QColor(up_back_ground_color)));
        painter.drawPolygon(&frame_points.front(),frame_points.size());
    };

    //draw after piece
    auto drawAfterPiece = [&](int pnum){
            painter.setPen(QPen(QBrush(Qt::black),grid_size*0.1)); // draw piece
            painter.setBrush(QBrush(QColor(colors[pnum][0],colors[pnum][1],colors[pnum][2], 255)));
//            int pcount = field.getPiece(pnum).getSize();
//            QPointF points[pcount];
//            for(int tes = 0;tes < pcount; tes++){
//                points[tes] = getPosition(field.getPiece(pnum).getPolygon().outer().at(tes));
//            }
//            painter.drawPolygon(points,pcount);
            std::vector<QPointF> points;
            for(auto point : field.getPieces().at(pnum).getPolygon().outer()){
                points.push_back(getPosition(point));
            }
            painter.drawPolygon(&points.front(),points.size());
            //draw piece id
            painter.setFont(QFont("Decorative", grid_size*3, QFont::Thin)); // text font
            painter.setBackgroundMode(Qt::OpaqueMode);
            painter.setBackground(QBrush(Qt::white));
            painter.setPen(QPen(QBrush(Qt::red), 0.5));
            //centroidで中心にidを描画
            point_i center;
            boost::geometry::centroid(field.getPiece(pnum).getPolygon(),center);
            QPointF piececenter = getPosition(center);
            painter.drawText(piececenter, QString(QString::number(field.getPiece(pnum).getId())));// draw
            //draw corner begin id
            painter.setPen(QPen(QBrush(Qt::black), 0.5));
            QPointF corner_begin = getPosition(field.getPiece(pnum).getPolygon().outer().at(0));
            corner_begin.setX(corner_begin.x() < piececenter.x()
                              ? corner_begin.x() + grid_size * 3
                              : corner_begin.x() + grid_size * 3 );
            corner_begin.setY(corner_begin.y() < piececenter.y()
                              ? corner_begin.y() + grid_size * 3
                              : corner_begin.y() + grid_size * 3 );
            painter.drawText(corner_begin, QString("s"+QString::number(field.getPiece(pnum).getId())));
            //draw evalution
            painter.setBackgroundMode(Qt::TransparentMode);
            QColor evalution_color = {255,0,255};
            painter.setPen(QPen(QBrush(evalution_color),10));
            painter.setFont(QFont("Deciratuve",grid_size*5,QFont::Bold));
            QPointF evalution_point = {window_width/15,window_height/15};
            painter.drawText(evalution_point, QString::number(field.getTotalEvaluation())+"     :     "+QString::number(field.getFrame().getJointedPieces().size())+"/"+QString::number(field.getElementaryPieces().size()));
            //painter.drawText(display_pos, QString::number(field->getTotalEvaluation())+"     :     "+QString::number(field->getFrame().getJointedPieces().size())+"/"+QString::number(field->getElementaryPieces().size()));
            //draw number

    };

    //draw down backdround
    auto drawDownBackground = [&]{
        painter.setBrush(QBrush(QColor(Qt::white)));
        painter.drawRect(QRect(left_right_margin,
                               down_up_y,
                               grid_col*grid_size,
                               grid_row*grid_size));
    };

    auto drawBeforePiece = [&](int pnum){
        painter.setPen(QPen(QBrush(Qt::black),grid_size*0.1));
        painter.setBrush(QBrush(QColor(colors[pnum][0],colors[pnum][1],colors[pnum][2], 255)));
        int pcount = field.getPiece(pnum).getSize();
        QPointF points[pcount];
        for(int tes = 0;tes < pcount; tes++){
            points[tes] = getPiecePosition(field.getPiece(pnum).getPolygon().outer().at(tes));
        }
        painter.drawPolygon(points,pcount);
    };

    auto drawProcessingLine = [&](int pnum){
        boost::geometry::centroid(field.getPiece(pnum).getPolygon(),center);
        QPointF afterpiececenter = getPosition(center);
        QPointF beforepiececenter = getPiecePosition(center);
        painter.setPen(QPen(QBrush(Qt::yellow), 0.5));
        painter.drawLine(afterpiececenter, beforepiececenter);
    };


    drawFrame();
    drawDownBackground();
    for(int piece_num = 0; piece_num < field.getPieces().size(); piece_num++){
            drawAfterPiece(piece_num);
            drawBeforePiece(piece_num);
            drawProcessingLine(piece_num);
    }
    drawGrid();
}
QPointF NeoAnswerBoard::getPosition(point_i point){//point_iを上画面のgridと対応させるようにQPointFに変換する
    return QPointF(left_right_margin + point.x() * grid_size, top_bottom_margin + point.y() * grid_size);
}

QPointF NeoAnswerBoard::getPiecePosition(point_i point)
{
    return QPointF(left_right_margin + point.x() * grid_size, down_up_y + point.y() * grid_size);
}

void NeoAnswerBoard::setField(procon::NeoField input_field){//fieldを設定
    field=input_field;
}

void NeoAnswerBoard::firstField(){//初期状態でのfieldを設定(実際は使わない)
    procon::NeoField inpfield;
    procon::NeoExpandedPolygon polygon;
    procon::NeoExpandedPolygon poly0;
    procon::NeoExpandedPolygon poly1;
    procon::NeoExpandedPolygon poly2;
    std::vector<polygon_i> piecepolygon(3);
    polygon_i framepolygon;

    framepolygon.outer().push_back(point_i(5,5));
    framepolygon.outer().push_back(point_i(85,7));
    framepolygon.outer().push_back(point_i(75,48));
    framepolygon.outer().push_back(point_i(18,57));
    framepolygon.outer().push_back(point_i(5,5));
    polygon.resetPolygonForce(framepolygon);
    inpfield.setFrame(polygon);

    piecepolygon[0].outer().push_back(point_i(5,5));
    piecepolygon[0].outer().push_back(point_i(45,6));
    piecepolygon[0].outer().push_back(point_i(25,15));
    piecepolygon[0].outer().push_back(point_i(5,5));
    polygon.resetPolygonForce(piecepolygon[0]);
    inpfield.setPiece(polygon);

    piecepolygon[1].outer().push_back(point_i(45,6));
    piecepolygon[1].outer().push_back(point_i(65,36));
    piecepolygon[1].outer().push_back(point_i(45,35));
    piecepolygon[1].outer().push_back(point_i(45,6));
    polygon.resetPolygonForce(piecepolygon[1]);
    inpfield.setPiece(polygon);

    piecepolygon[2].outer().push_back(point_i(12,32));
    piecepolygon[2].outer().push_back(point_i(15,21));
    piecepolygon[2].outer().push_back(point_i(35,23));
    piecepolygon[2].outer().push_back(point_i(44,35));
    piecepolygon[2].outer().push_back(point_i(32,45));
    piecepolygon[2].outer().push_back(point_i(12,32));
    polygon.resetPolygonForce(piecepolygon[2]);
    inpfield.setPiece(polygon);

    setField(inpfield);
};
