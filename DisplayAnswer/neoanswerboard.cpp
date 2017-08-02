#include "neoanswerboard.h":
#include "ui_neoanswerboard.h"

NeoAnswerBoard::NeoAnswerBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NeoAnswerBoard)
{
    ui->setupUi(this);
    //firstField();
}

NeoAnswerBoard::~NeoAnswerBoard()
{
    delete ui;
}

void NeoAnswerBoard::setDockMode(bool inp){
    single_mode = inp;
}


void NeoAnswerBoard::paintEvent(QPaintEvent *event)
{
    const QString up_back_ground_color = "#7BAB4F";
    const QString down_back_ground_color = "#118822";
    const int window_width = this->width();
    const int window_height = this->height();

    //色リスト
    QStringList colorlist;
        colorlist << "#f39700"
                  << "#e60012"
                  << "#9caeb7"
                  << "#00a7db"
                  << "#00a7db"
                  << "#d7c447"
                  << "#9b7cb6"
                  << "#00ada9"
                  << "#bb641d"
                  << "#e85298"
                  << "#0079c2"
                  << "#6cbb5a"
                  << "#b6007a"
                  << "#e5171f"
                  << "#522886"
                  << "#0078ba"
                  << "#019a66"
                  << "#e44d93"
                  << "#814721"
                  << "#a9cc51"
                  << "#ee7b1a"
                  << "#00a0de"
                  << "#dd3333"
                  << "#bb3322"
                  << "#bb3377"
                  << "#773333"
                  << "#ee7733"
                  << "#ffdd00"
                  << "#ffdd99"
                  << "#bbbb99"
                  << "#88bb88"
                  << "#669966"
                  << "#66aa88"
                  << "#99dd66"
                  << "#339966"
                  << "#119988"
                  << "#114433"
                  << "#225588"
                  << "#44aacc"
                  << "#333388"
                  << "#ff89ff"
                  << "#ff8989"
                  << "#99ffcc"
                  << "#9eff9e"
                  << "#add6ff"
                  << "#ffcc99"
                  << "#00ff00"
                  << "#8b0000"
                  << "#8b008b"
                  << "#00ffff";
    //色リストをQVectorに格納
    QVector<QString> list = colorlist.toVector();

    // 101 x 65
    frame_margin = 16; //枠の最低限の幅(左右の合計)
    const int grid_row = 65 + frame_margin;
    const int grid_col = 101 + frame_margin;
    const int grid_margin = 1;
    const int splitedheight = (single_mode==true
                               ?window_height
                               :window_height/2);
    grid_size =
            window_width <= splitedheight
            ? window_width / (grid_col + grid_margin)
            : splitedheight / (grid_row + grid_margin);
    //grid_size -= grid_size*8;
    top_bottom_margin = (splitedheight - grid_size * grid_row) / 2;
    left_right_margin = (window_width - grid_size * grid_col) / 2;
    down_up_y = splitedheight + top_bottom_margin;

    //余白(枠の部分)を設定
    //top_bottom_margin += grid_size*8;
    //left_right_margin += grid_size*8;

    QPainter painter(this);

    //背景を描画
    painter.setBrush(QBrush(QColor(up_back_ground_color)));
    painter.drawRect(QRect(0,0,window_width,splitedheight));
    painter.setBrush(QBrush(QColor(down_back_ground_color)));
    painter.drawRect(QRect(0, splitedheight, window_width, window_height));


    //グリッドを描画
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

    //上画面フレームを描画
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

        std::vector<procon::NeoExpandedPolygon> frame__;
        frame__ = field.getFrame();
        for(auto frame_ : frame__ ){
            std::vector<QPointF> frame_points;
            for(auto point : frame_.getPolygon().outer()){
                frame_points.push_back(getPosition(point));
            }
            painter.setBrush(QBrush(QColor(up_back_ground_color)));
            painter.drawPolygon(&frame_points.front(),frame_points.size());
        }
    };
    //処理後ピースを描画
    auto drawAfterPiece = [&](int pnum){
            painter.setPen(QPen(QBrush(Qt::black),grid_size*0.1)); // draw piece
            painter.setBrush(QBrush(QColor(list[pnum])));
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
    };

    //下画面フレームを描画
    auto drawDownBackground = [&]{
        painter.setBrush(QBrush(QColor(Qt::white)));
        painter.drawRect(QRect(left_right_margin,
                               down_up_y,
                               grid_col*grid_size,
                               grid_row*grid_size));
    };

    //処理前ピースを描画
    auto drawBeforePiece = [&](int pnum){
        painter.setPen(QPen(QBrush(Qt::black),grid_size*0.1));
        painter.setBrush(QBrush(QColor(list[pnum])));
        int pcount = field.getPiece(pnum).getSize();
        QPointF points[pcount];
        for(int tes = 0; tes < pcount; tes++){
            points[tes] = getPiecePosition(field.getPiece(pnum).getPolygon().outer().at(tes));
        }
        painter.drawPolygon(points,pcount);
    };

    //ピースIdを描画
    auto drawPieceId = [&](int pnum){
        painter.setFont(QFont("Decorative", grid_size*2, QFont::Thin)); // text font
        painter.setBackgroundMode(Qt::OpaqueMode);
        painter.setBackground(QBrush(QColor(list[pnum])));
        painter.setPen(QPen(QBrush(Qt::white), 0.3));
        point_i center;
        boost::geometry::centroid(field.getPiece(pnum).getPolygon(),center);
        QPointF piececenter = getPosition(center);
        piececenter.setX(piececenter.x() - grid_size);
        piececenter.setY(piececenter.y() + grid_size);
        painter.drawText(piececenter, QString(QString::number(field.getPiece(pnum).getId())));
    };

    //処理線を描画
    auto drawProcessingLine = [&](){
        point_i center;
        boost::geometry::centroid(polygon_list[point_id], center);
        QPointF aftercentroid = getPosition(center);
        QPointF beforecentroid = getPiecePosition(center);
        if(selecter == true) painter.setPen(QPen(QBrush(Qt::red), 1.0));
        if(selecter == false) painter.setPen(QPen(QBrush(Qt::blue), 1.0));
        painter.drawLine(aftercentroid, beforecentroid);
    };

    //評価値を描画
    auto drawEvalution = [&]{
        painter.setBackgroundMode(Qt::TransparentMode);
        QColor evalution_color = {255,0,255};
        painter.setPen(QPen(QBrush(evalution_color),10));
        //painter.setFont(QFont("Deciratuve",grid_size*5,QFont::Bold));
        int evalution_size = (top_bottom_margin > left_right_margin*5
                    ?top_bottom_margin/3
                    :left_right_margin/6);
        if(window_width < evalution_size * 6)evalution_size = window_width / 6;
        painter.setFont(QFont("Deciratuve",evalution_size,QFont::Bold));
        QPointF evalution_point = {grid_size , evalution_size * 2};
        int jointed_piece_total = 0;
        std::vector<procon::NeoExpandedPolygon> frame__;
        frame__ = field.getFrame();
        for(auto frame_ : frame__){
            jointed_piece_total += frame_.getJointedPieces().size();
        }
        painter.drawText(evalution_point, QString::number(field.getTotalEvaluation())+" : "+QString::number(jointed_piece_total)+"/"+QString::number(field.getElementaryPieces().size()));
        //painter.drawText(evalution_point, QString::number(field.getTotalEvaluation())+" : "+QString::number(field.getFrame().getJointedPieces().size())+"/"+QString::number(field.getElementaryPieces().size()));
        //painter.drawText(display_pos, QString::number(field->getTotalEvaluation())+" : "+QString::number(field->getFrame().getJointedPieces().size())+"/"+QString::number(field->getElementaryPieces().size()));
    };

    drawFrame();
    drawDownBackground();
    for(int piece_num = 0; piece_num < field.getPieces().size(); piece_num++){
        drawAfterPiece(piece_num);
        drawBeforePiece(piece_num);
    }
    for(int piece_num =0; piece_num < field.getPieces().size();piece_num++){
        drawPieceId(piece_num);
    }

    if(paintif){
        if(point_id != -1){
            drawProcessingLine();
            paintif = false;
        }
    }

    drawEvalution();
    drawGrid();
}

void NeoAnswerBoard::keyPressEvent(QKeyEvent *event)
{
    int hoge = field.getPieces().size();
    if(point_id < hoge - 1){
        paintif = true;
        if(event->key() == Qt::Key_L){
            selecter = true;
            point_id++;
        }
        if(event->key() == Qt::Key_P){
            selecter = false;
            point_id++;
        }
        this->update();
    }
}

QPointF NeoAnswerBoard::getPosition(point_i point){//point_iを上画面のgridと対応させるようにQPointFに変換する
    int pointx = point.x() + frame_margin/2;
    int pointy = point.y() + frame_margin/2;
    return QPointF(left_right_margin + pointx * grid_size, top_bottom_margin + pointy * grid_size);
}

QPointF NeoAnswerBoard::getPiecePosition(point_i point){//point_iを下画面の枠と対応させるようにQPointFに変換する
    int pointx = point.x() + frame_margin/2;
    int pointy = point.y() + frame_margin/2;
    return QPointF(left_right_margin + pointx * grid_size, down_up_y + pointy * grid_size);
}

void NeoAnswerBoard::setField(procon::NeoField input_field){//fieldを設定
    field=input_field;
    paintif = true;
    this->update();

    polygon_list.clear();
    std::vector<procon::NeoExpandedPolygon> pieces = field.getPieces();
    std::sort(pieces.begin(), pieces.end(), [](const procon::NeoExpandedPolygon& a, const procon::NeoExpandedPolygon& b)->bool{
        return a.getId() < b.getId();
    });
    for(auto piece : pieces){
        if(piece.getId() != -1) polygon_list.push_back(piece.getPolygon());
        std::cout << piece.getId() << std::endl;
    }
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
    std::vector<procon::NeoExpandedPolygon> polygonvec;
    polygonvec.push_back(polygon);
    inpfield.setFrame(polygonvec);

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
