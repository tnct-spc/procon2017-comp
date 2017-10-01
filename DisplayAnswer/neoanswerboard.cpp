#include <string>
#include "neoanswerboard.h"
#include "ui_neoanswerboard.h"
// TODO:painteventが呼ばれるとピースが描画されなくなるバグを修正

#include "neopolygonio.h"

#include "neopolygonio.h"
#include "neoanswerdock.h"
#include "neopolygonviewer.h"

NeoAnswerBoard::NeoAnswerBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NeoAnswerBoard)
{
    ui->setupUi(this);
}

NeoAnswerBoard::~NeoAnswerBoard()
{
    delete ui;
}

void NeoAnswerBoard::setSingleMode(bool inp){
    single_mode = inp;
}

void NeoAnswerBoard::setShowUnplacedPieces(bool input){
    showunplacedpieces = input;
}

void NeoAnswerBoard::setSelectPieceMode(bool mode)
{
    this->select_piece_mode = mode;
}

void NeoAnswerBoard::setUp()
{
    QMessageBox msgBox;
    msgBox.setText("ピースの表示モードを選択");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, tr("ピースを順送りにする"));
    msgBox.setButtonText(QMessageBox::No,  tr("ピースを全て表示する"));
    msgBox.setIcon(QMessageBox::Warning);
    int res = msgBox.exec();
    if(res == QMessageBox::Yes) allif = true;
    if(res == QMessageBox::No) allif = false;
    this->setField(field);
}

void NeoAnswerBoard::setText(std::string text){
    output_string = QString::fromStdString(text);
}

void NeoAnswerBoard::paintEvent(QPaintEvent *event)
{
    const QString up_back_ground_color = "#7BAB4F";
    const QString down_back_ground_color = "#118822";
    const QString unplaced_back_ground_color = "#EEEEEE";
    const int width_size = (showunplacedpieces
                ? this->width() * 0.6
                : this->width());
    const int window_height = this->height();
    const int window_width = this->width();

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
            width_size <= splitedheight
            ? width_size / (grid_col + grid_margin)
            : splitedheight / (grid_row + grid_margin);
    //grid_size -= grid_size*8;
    top_bottom_margin = (splitedheight - grid_size * grid_row) / 2;
    left_right_margin = (showunplacedpieces
            ? (width_size - grid_size * grid_col) / 2
            : (width_size - grid_size * grid_col) / 2);
    down_up_y = splitedheight + top_bottom_margin;

    //余白(枠の部分)を設定
    //top_bottom_margin += grid_size*8;
    //left_right_margin += grid_size*8;

    QPainter painter(this);

    //背景を描画
    painter.setBrush(QBrush(QColor(up_back_ground_color)));
    painter.drawRect(QRect(0,0,width_size,splitedheight));
    painter.setBrush(QBrush(QColor(down_back_ground_color)));
    painter.drawRect(QRect(0, splitedheight, width_size, window_height));

    painter.setBrush(QBrush(QColor(unplaced_back_ground_color)));
    painter.drawRect(QRect(width_size,0,window_width,splitedheight));

    //グリッドを描画
    auto drawGrid = [&]{
        painter.setPen(QPen(QBrush(Qt::black),0.1));
        for (int current_col = 0; current_col < grid_col + 1; ++current_col) {
            int x = current_col * this->grid_size + left_right_margin;
            painter.drawLine(x,top_bottom_margin,x,splitedheight - top_bottom_margin);
        }
        for (int current_row = 0; current_row < grid_row + 1; ++current_row) {
            int y = current_row * grid_size + top_bottom_margin;
            painter.drawLine(left_right_margin,y,width_size - left_right_margin,y);
        }
    };

    //上画面フレームを描画
    auto drawFrame = [&]{
        painter.setBrush(QBrush(QColor(236,182,138, 200))); //frame color
        painter.setPen(QPen(QBrush(Qt::black),grid_size*0.1));
        painter.drawRect(QRect(left_right_margin,top_bottom_margin,grid_col*grid_size,grid_row*grid_size));


        for(auto& frame : field.getElementaryFrame() ){
            std::vector<QPointF> frame_points;
            for(auto point : frame.getPolygon().outer()){
                frame_points.push_back(getPosition(point));
            }
            painter.setBrush(QBrush(QColor(up_back_ground_color)));
            painter.drawPolygon(&frame_points.front(),frame_points.size());
        }
    };

    //処理後ピースを描画
    auto drawAfterPiece = [&](procon::NeoExpandedPolygon expanded_poly){
            painter.setPen(QPen(QBrush(Qt::black),grid_size*0.1)); // draw piece
            if(std::find(clicked_piece_id.begin(),clicked_piece_id.end(),expanded_poly.getId()) != clicked_piece_id.end())painter.setBrush(QBrush(Qt::white));
            else painter.setBrush(QBrush(QColor(list[expanded_poly.getId()])));
//          int pcount = field.getPiece(pnum).getSize();
//          QPointF points[pcount];
//          for(int tes = 0;tes < pcount; tes++){
//              points[tes] = getPosition(field.getPiece(pnum).getPolygon().outer().at(tes));
//          }
//          painter.drawPolygon(points,pcount);
            std::vector<QPointF> points;
            for(auto point : expanded_poly.getPolygon().outer()){
                points.push_back(getPosition(point));
            }
            painter.drawPolygon(&points.front(),points.size());
    };


    //処理前ピースを描画
    auto drawBeforePiece = [&](procon::ExpandedPolygon expanded_poly){
        polygon_t poly = expanded_poly.getPolygon();
        painter.setPen(QPen(QBrush(Qt::black),grid_size*0.1));
        painter.setBrush(QBrush(QColor(list[expanded_poly.getId()])));
        std::vector<QPointF> points;
        for(auto point : poly.outer()){
            points.push_back(getPiecePosition(point));
        }
        painter.drawPolygon(&points.front(),points.size());
    };

    //ピースIdを描画
    auto drawPieceId = [&](procon::NeoExpandedPolygon expanded_poly){
        painter.setFont(QFont("Decorative", grid_size*3, QFont::Thin)); // text font
        painter.setBackgroundMode(Qt::OpaqueMode);
        painter.setBackground(QBrush(QColor(list[expanded_poly.getId()])));
        painter.setPen(QPen(QBrush(Qt::white), 0.3));
        point_i center;
        boost::geometry::centroid(expanded_poly.getPolygon(), center);
        QPointF piececenter = getPosition(center);
        piececenter.setX(piececenter.x() - grid_size);
        piececenter.setY(piececenter.y() + grid_size);
        painter.drawText(piececenter, QString(QString::number(expanded_poly.getId())));
    };

    auto drawInverseMark = [&](procon::NeoExpandedPolygon neopoly){//裏返されてるなら@マークをつける
        polygon_i poly = neopoly.getPolygon();
        painter.setFont(QFont("Decorative", grid_size*3, QFont::Thin)); // text font
        painter.setBackgroundMode(Qt::OpaqueMode);
        painter.setBackground(QBrush(QColor(list[neopoly.getId()])));
        painter.setPen(QPen(QBrush(Qt::red), 0.3));
        point_i cent;
        bg::centroid(poly,cent);
        QPointF cent_point = getPosition(cent);
        cent_point.setX(cent_point.x() + grid_size);
        cent_point.setY(cent_point.y() - grid_size);
        painter.drawText(cent_point, QString(QString("@")));
    };

    auto drawProcessingLine = [&](procon::NeoExpandedPolygon neoexpanded_poly, bool color){//引数かえて書き直した
        if(!single_mode){

           int poly_id = neoexpanded_poly.getId();
           point_i up_center;
           point_t down_center;
           procon::ExpandedPolygon expanded_poly;
           for(auto sc_poly : scanned_poly){
               if(sc_poly.getId() == poly_id)expanded_poly = sc_poly;
           }



           bg::centroid(neoexpanded_poly.getPolygon(), up_center);
           bg::centroid(expanded_poly.getPolygon(), down_center);//ここでcentroidくんがちゃんと出せてない感じあります

           QPointF aftercentroid = getPosition(up_center);
           QPointF beforecentroid = getPiecePosition(down_center);
           if(color){
               painter.setPen(QPen(QBrush(Qt::blue), 2.0));
           }else{
               painter.setPen(QPen(QBrush(Qt::red), 2.0));
           }
           painter.drawLine(aftercentroid, beforecentroid);


        }
    };
    //頂点番号を描画
    auto drawPolygonPointNum = [&]{
        if(single_mode){
        painter.setFont(QFont("Decorative", grid_size*2, QFont::Thin)); // text font
        painter.setBackgroundMode(Qt::OpaqueMode);
        painter.setBackground(QBrush(Qt::white));

//        int number=0;
//        for(auto expanded_polygon: field.getPieces()){
//            polygon_i poly = expanded_polygon.getPolygon();
//            painter.setPen(QPen(QBrush(QColor(list[expanded_polygon.getId()])), 0.1));
//            for(auto point : poly.outer()){
//                QPointF text_point = getPosition(point);
//                painter.drawText(text_point,QString::number(expanded_polygon.getId()));
//            }
//            ++number;
//        }
        painter.setPen(QPen(QBrush(QColor(236,182,138, 200)),0.1));
        int count=0;
        for(auto frame : field.getFrame()){
            for(auto point : frame.getPolygon().outer()){
                QPointF text_point = getPosition(point);
                painter.drawText(text_point,QString::number(count));
                ++count;
            }
        }

        }
    };

    //評価値を描画 設定されたtextもここで描画する
    auto drawEvalution = [&]{
        painter.setBackgroundMode(Qt::TransparentMode);
        QColor evalution_color = {255,0,255};
        painter.setPen(QPen(QBrush(evalution_color),10));
        //painter.setFont(QFont("Deciratuve",grid_size*5,QFont::Bold));
        int evalution_size = (top_bottom_margin > left_right_margin*5
                    ?top_bottom_margin/3
                    :left_right_margin/6);
        if(width_size < evalution_size * 6)evalution_size = width_size / 6;
        painter.setFont(QFont("Deciratuve",evalution_size,QFont::Bold));
        QPointF evalution_point;
        evalution_point.setX(grid_size);
        evalution_point.setY(evalution_size * 2);
       painter.drawText(evalution_point, QString::number(field.getFrame().size()) + "  " + QString::number(field.getTotalEvaluation())+" : "+QString::number(field.getPieces().size())+"/"+QString::number(field.getElementaryPieces().size()));

        QPointF text_point = evalution_point;
        painter.setFont(QFont("Deciratuve",10,QFont::AnyStyle));
        text_point.setY(splitedheight - 60);
        evalution_color = {0,0,0};
        painter.setPen(QPen(QBrush(evalution_color),10));
        painter.drawText(text_point, output_string);
        //painter.drawText(evalution_point, QString::number(field.getTotalEvaluation())+" : "+QString::number(field.getFrame().getJointedPieces().size())+"/"+QString::number(field.getElementaryPieces().size()));
        //painter.drawText(display_pos, QString::number(field->getTotalEvaluation())+" : "+QString::number(field->getFrame().getJointedPieces().size())+"/"+QString::number(field->getElementaryPieces().size()));
    };

    drawFrame();
    //drawDownBackground();

    for(auto poly : field.getPieces()){//ここの描画はsingle_modeやキーの状況に問わずやるらしい
            drawAfterPiece(poly);
    }
    for(auto poly : field.getPieces()){
        drawPieceId(poly);
        if(poly.is_inverse)drawInverseMark(poly);
    }
    if(!single_mode){
        for(auto poly : scanned_poly){
            drawBeforePiece(poly);
        }
      //  for(unsigned int piece_num = 0; piece_num < field.getPieces().size(); ++piece_num){
      //      drawBeforePiece(piece_num);
      //  }

        if(blue_id != -1){
                    drawProcessingLine(sorted_poly.at(blue_id), true);
                    drawProcessingLine(sorted_poly.at(red_id), false);
        }
    }

    auto drawunplacedpieces = [&]{//人力beamsearch用

        int unplacedpiececount = field.getElementaryPieces().size() - field.getPieces().size();

        auto getUnplacedPosition = [&](point_i point,int value){

            QPointF q_point = getPosition(point);
            if(value * 2 < unplacedpiececount){
                q_point.setX(q_point.x() + width_size);
                q_point.setY(point.y()*grid_size + window_height*0.5 + (unplacedpiececount/4 - value) * window_height/unplacedpiececount*1.4);
            }else{
                q_point.setX(q_point.x() + width_size + (window_width - width_size) / 2);
                q_point.setY(point.y()*grid_size + window_height*0.5 + (unplacedpiececount/4 - (value - unplacedpiececount/2)) * window_height/unplacedpiececount*1.4);
            }

            return q_point;
        };

        auto drawPiece = [&](polygon_i poly,int id,int count){
            painter.setPen(QPen(QBrush(Qt::black),grid_size*0.1)); // draw piece
            painter.setBrush(QBrush(QColor(list[id])));
            std::vector<QPointF> points;

            //中央に揃える処理
            point_i center_point;
            polygon_i center_poly;
            bg::centroid(poly , center_point);
            bg::strategy::transform::translate_transformer<int,2,2>translate(-center_point.x(),-center_point.y());
            bg::transform(poly,center_poly,translate);

            for(auto point : center_poly.outer()){
                points.push_back(getUnplacedPosition(point,count));
            }
            painter.drawPolygon(&points.front(),points.size());

        };


        int value=1;
        for(unsigned int count=0;count<field.getElementaryPieces().size();++count){
            if(!field.getIsPlaced().at(count)){//置かれてないなら
                polygon_i poly = field.getElementaryPieces().at(count).getPolygon();
                int poly_id = field.getElementaryPieces().at(count).getId();
                drawPiece(poly,poly_id,value);
                ++value;
            }
        }
    };

    drawPolygonPointNum();
    drawEvalution();
    drawGrid();
    if(showunplacedpieces)drawunplacedpieces();

}

void NeoAnswerBoard::keyPressEvent(QKeyEvent *event)
{
    if( !field.getPieces().empty() ){
        /*
        int hoge = field.getPieces().size() - 1;
        if(point_id > 0){
            if(point_id < hoge){
                paintif = true;
                if(red_id < hoge){
                    if(event->key() == Qt::Key_A){
                        selecter = true;
                        ++red_id;
                        ++point_id;
                    }
                }

                if(blue_id < hoge){
                    if(event->key() == Qt::Key_L){
                        selecter = false;
                        ++blue_id;
                        ++point_id;
                    }
                }
            }
        }

        if(point_id > 0){
            if(point_id < hoge + 1){
                paintif = true;
                if(red_id > 0){
                    if(event->key() == Qt::Key_S){
                        selecter = true;
                        --red_id;
                        --point_id;
                    }
                }

                if(blue_id > 0){
                    if(event->key() == Qt::Key_K){
                        selecter = false;
                        --blue_id;
                        --point_id;
                    }
                }
            }
        }*/
        if(event->key() == Qt::Key_0){
            std::cout << "push 0" << red_id << "   " << blue_id << std::endl;
        }

        if(event->key() == Qt::Key_A && red_id!=0){
            --red_id;
        }
        if(event->key() == Qt::Key_S){
            if(red_id != blue_id - 1)++red_id;
        }

        if(event->key() == Qt::Key_K){
            if(red_id != blue_id - 1)--blue_id;
        }
        if(event->key() == Qt::Key_L && blue_id!=sorted_poly.size() - 1){
            ++blue_id;
        }
    }


    this->update();





}

void NeoAnswerBoard::setScannedPieces(std::vector<procon::ExpandedPolygon> vec){
    scanned_poly = vec;
    double poly_size=0;
    for(auto expanded_poly : scanned_poly){

        polygon_t poly = expanded_poly.getPolygon();
        double max_x=-10000;//ここ頭悪い
        double max_y=-10000;
        for(auto point : poly.outer()){
            if(point.x() > max_x)max_x=point.x();
            if(point.y() > max_y)max_y=point.y();

        }

        double check_poly_size = (max_x / 101 > max_y / 65
                      ? (max_x) / 101
                      : (max_y) / 65
                      );

        if(check_poly_size>poly_size)poly_size = check_poly_size;
    }
    for(auto& expanded_poly : scanned_poly){
        polygon_t poly = expanded_poly.getPolygon();

        polygon_t trans_poly;
        bg::strategy::transform::scale_transformer<double , 2, 2> trans_scale(1 / poly_size);//逆数を使って適当に設定
        bg::transform(poly,trans_poly,trans_scale);
        procon::ExpandedPolygon buf(expanded_poly.getId());//一時的に格納して後でpush_backするやつ
        buf.resetPolygonForce(trans_poly);
        expanded_poly = buf;
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

QPointF NeoAnswerBoard::getPiecePosition(point_t point){//point_tを下画面の枠と対応させるようにQPointFに変換する
    int pointx = point.x() + frame_margin/2;
    int pointy = point.y() + frame_margin/2;
    return QPointF(left_right_margin + pointx * grid_size, down_up_y + pointy * grid_size);
}

void NeoAnswerBoard::setField(procon::NeoField input_field){//fieldを設定

    field=input_field;

    /*
    if( !field.getPieces().empty() ){
        paintif = true;
        polygon_list.clear();
        std::vector<procon::NeoExpandedPolygon> pieces = field.getPieces();
        std::sort(pieces.begin(), pieces.end(), [](const procon::NeoExpandedPolygon& a, const procon::NeoExpandedPolygon& b)->bool{
            point_i center1;
            point_i center2;
            boost::geometry::centroid(a.getPolygon(),center1);
            boost::geometry::centroid(b.getPolygon(),center2);
            return center1.x() < center2.x();
        });
        for(auto piece : pieces){
            polygon_list.push_back(piece.getPolygon());
        }
        this->update();
    }*/

    for(auto frame :field.getElementaryFrame()){
        for(auto point : frame.getPolygon().outer()){
            if(point.x() > 101 || point.x() < 0)std::cout << "framepoint x error!!!!!!!!!!" << std::endl;
            if(point.y() > 65 || point.y() < 0)std::cout << "framepoint y error!!!!!!!!!!" << std::endl;
        }
    }
    sorted_poly.clear();

    for(auto expanded_poly : field.getPieces()){//sorted_polyにgetPiecesの中身をソートして投入する
        polygon_i poly = expanded_poly.getPolygon();
        point_i centroid_point;
        bg::centroid(poly,centroid_point);
        bool flag=false;
        for(unsigned int count=0;count<sorted_poly.size();++count){
            procon::NeoExpandedPolygon sort_poly = sorted_poly.at(count);
            point_i sort_centroid_point;
            bg::centroid(sort_poly.getPolygon(),sort_centroid_point);
            if(centroid_point.x() < sort_centroid_point.x()){
                sorted_poly.insert(sorted_poly.begin() + count,expanded_poly);
                flag=true;
                break;
            }
        }
        if(!flag)sorted_poly.push_back(expanded_poly);
    }

//    for(auto poly : sorted_poly){
//        point_i center_point;
//        bg::centroid (poly.getPolygon(),center_point);
//        std::cout << bg::dsv(center_point) << std::endl;
//    }

    blue_id = sorted_poly.size()-1;
}

//#define OUT_FILE

void NeoAnswerBoard::mousePressEvent(QMouseEvent *event)
{

//#define DEBUG
#ifdef DEBUG
    if(event->button() == Qt::MouseButton::RightButton){
        NeoPolygonIO::exportPolygon(this->field,"/home/yui/Procon/fieldcdv/debug-field.csv");
    }else if(event->button() == Qt::MouseButton::LeftButton){
        //二回目以上クリックされたら困る
        if(this->field.evaluate_cache.empty()) return;

        NeoAnswerDock *nad = new NeoAnswerDock();
        nad->setWindowTitle("career");
        nad->show();

        //fieldのリセット
        auto reset_neo_field = [](procon::NeoField input_field){
            procon::NeoField output_field;
            output_field.setFrame(input_field.getElementaryFrame());
            output_field.setElementaryFrame(input_field.getElementaryFrame());
            output_field.setElementaryPieces(input_field.getElementaryPieces());
            return output_field;
        };
        procon::NeoField newField = reset_neo_field(this->field);

        int count = 0;
        for(Evaluate i : this->field.evaluate_cache){
            count++;
            std::tuple<std::vector<procon::NeoExpandedPolygon> , procon::NeoExpandedPolygon , bool> connected = PolygonConnector::connect(
                        newField.getFrame().at(i.frame_index),
                        i.is_inversed
                            ? newField.getElementaryInversePieces().at(i.piece_index)
                            : newField.getElementaryPieces().at(i.piece_index),
                        i.connection
            );
            std::string text =  "fs:" + std::to_string(i.connection.frame_side_index) +
                                ",ps:" + std::to_string(i.connection.polygon_side_index) +
                                ",fp:" + std::to_string(i.connection.frame_point_index) +
                                ",pp:" + std::to_string(i.connection.polygon_point_index);
            std::cout<<text<<std::endl;
            std::vector<procon::NeoExpandedPolygon> field_frame = newField.getFrame();
            int j = 0;
            for(procon::NeoExpandedPolygon frame : field_frame) {
                NeoPolygonViewer::getInstance().displayPolygon(frame.getPolygon(),"frame <" + std::to_string(j) + ">",false);
            }

            if(std::get<2>(connected)){
                std::vector<procon::NeoExpandedPolygon> newframe = std::get<0>(connected);
                for(procon::NeoExpandedPolygon nep : newframe){
                    field_frame.push_back(nep);
                }initialized
                field_frame.erase(field_frame.begin() + i.frame_index);
                newField.setFrame(field_frame);
                newField.setPiece(std::get<1>(connected));
                procon::NeoExpandedPolygon nep = std::get<1>(connected);
                NeoPolygonViewer::getInstance().displayPolygon(nep.getPolygon(),"hello",false);
                newField.setIsPlaced(i.piece_index);
                nad->addAnswer(newField , text);
#ifdef OUT_FILE
                std::string path = "/home/yui/Procon/fieldcdv/" + std::to_string(count) + ".csv";
                NeoPolygonIO::exportPolygon(newField , path);
#endif
            } else {
                std::cout << "結合失敗" << std::endl;
                nad->deleteLater();
                QMessageBox::warning(nad, tr("エラー"), tr("結合失敗！"));
                NeoPolygonViewer::getInstance().displayPolygon(i.is_inversed
                                                               ? newField.getElementaryInversePieces().at(i.piece_index).getPolygon()
                                                               : newField.getElementaryPieces().at(i.piece_index).getPolygon(),
                                                               "hello", false);
                break;
            }
        }
    }
#endif
    if(event->button() == Qt::MouseButton::LeftButton && this->select_piece_mode){
//        clickedpiece_id = -1;
        QPointF clickedposition = event->pos();
        //QPointF→point_iへの変換

        int pointx = clickedposition.x() - left_right_margin;
        int pointy = clickedposition.y() - top_bottom_margin;

        point_i clickedpos( pointx / grid_size - 8, pointy / grid_size - 8);
        std::cout << "clicked : " << bg::dsv(clickedpos) << std::endl;

        for(auto piece : field.getPieces()){
            if(bg::intersects(piece.getPolygon() , clickedpos)){
                std::cout << "piece id :" << piece.getId() << std::endl;

                auto index = std::find(clicked_piece_id.begin(),clicked_piece_id.end(),piece.getId());
                if(index == clicked_piece_id.end()){
                    this->clicked_piece_id.push_back(piece.getId());
                }else{
                    this->clicked_piece_id.erase(index);
                }
            }
        }
        this->update();
    }
}

std::vector<int> NeoAnswerBoard::getSelectedPieceId()
{
    return clicked_piece_id;
}
