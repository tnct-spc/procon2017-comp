#include "manpowerprob.h"
#include "ui_manpowerprob.h"

ManPowerProb::ManPowerProb(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ManPowerProb)
{
    ui->setupUi(this);
    standard_pos.set<0>(50);
    standard_pos.set<1>(32);
    logger = spdlog::get("ManPowerProb");
    not_put_part.outer().push_back(point_i(0,0));
    not_put_part.outer().push_back(point_i(101,0));
    not_put_part.outer().push_back(point_i(101,65));
    not_put_part.outer().push_back(point_i(0,65));
    not_put_part.outer().push_back(point_i(0,0));
    bg::correct(not_put_part);
}

ManPowerProb::~ManPowerProb()
{
    delete ui;
}

void ManPowerProb::paintEvent(QPaintEvent *event){

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

    QPainter painter(this);
    const QString back_ground_color = "#EEEEBB";
    const QString frame_color = "#DDDD99";
    const QString piece_color = "#992222";
    const int window_width = this->width();
    const int window_height = this->height();
    // 101 x 65
    const int grid_row = 65;
    const int grid_col = 101;
    const int grid_margin = 1;
    const int splitedheight = window_height;
    grid_size =
            window_width <= window_height
            ? window_width / (grid_col + grid_margin)
            : splitedheight / (grid_row + grid_margin);
    //grid_size -= grid_size*8;
    top_bottom_margin = (splitedheight - grid_size * grid_row) / 2;
    left_right_margin = (window_width - grid_size * grid_col) / 2;

    painter.setBrush(QBrush(QColor(back_ground_color)));
    painter.drawRect(QRect(0,0,window_width,window_height));

    auto drawGrid = [&]{
        painter.setPen(QPen(QBrush(Qt::black),0.3));
        for (int current_col = 0; current_col < grid_col + 1; ++current_col) {
            int x = current_col * grid_size + left_right_margin;
            painter.drawLine(x,top_bottom_margin,x,splitedheight - top_bottom_margin);
        }
        for (int current_row = 0; current_row < grid_row + 1; ++current_row) {
            int y = current_row * grid_size + top_bottom_margin;
            painter.drawLine(left_right_margin,y,window_width - left_right_margin,y);
        }
    };

    auto drawLastPolygon = [&]{
        painter.setPen(QPen(QBrush(Qt::red),5.0));
        std::vector<QPointF> draw_points;
        for(auto point : last_polygon.outer()){
            QPointF draw_point = translateToQPoint(point);
            painter.drawPoint(draw_point);
            draw_points.push_back(draw_point);
        }
        painter.setPen(QPen(QBrush(Qt::red),1.0));
        if(draw_points.size()>1){
            for(unsigned int count=0;count<draw_points.size() - 1;++count){
                painter.drawLine(draw_points.at(count),draw_points.at(count + 1));
            }
        }
        draw_points.clear();
    };

    auto drawFrames = [&]{
        painter.setPen(QPen(QBrush(Qt::black),1.0));
        QVector<QPointF> draw_points;
        painter.setBrush(QBrush(QColor(frame_color)));
        for(auto point : not_put_part.outer()){
            QPointF draw_point = translateToQPoint(point);
            draw_points.push_back(draw_point);
        }
        painter.drawPolygon(draw_points);
        draw_points.clear();
        painter.setBrush(QBrush(QColor(back_ground_color)));
        for(auto frame : frames){
            for(auto point : frame.outer()){
                QPointF draw_point = translateToQPoint(point);
                draw_points.push_back(draw_point);
            }
            painter.drawPolygon(draw_points);
            draw_points.clear();
        }
    };

    auto drawPieces = [&]{

        painter.setBrush(QBrush(QColor(piece_color)));
        QVector<QPointF> draw_points;
        for(auto piece : pieces){
            for(auto point : piece.outer()){
                QPointF draw_point = translateToQPoint(point);
                draw_points.push_back(draw_point);
            }
            painter.drawPolygon(draw_points);
            draw_points.clear();
        }
    };


    drawFrames();
    drawPieces();
    drawLastPolygon();
    drawGrid();
}
void ManPowerProb::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::MouseButton::LeftButton){
        QPointF clickedposition = event->pos();
        point_i clicked = translateToPointi(clickedposition);

        if(is_frame){
            if(checkCanPlaceFrame(clicked))last_polygon.outer().push_back(clicked);
            else logger->error("範囲外を指定しています");
        }
        else{
            if(checkCanPlacePiece(clicked))last_polygon.outer().push_back(clicked);
            else logger->error("範囲外を指定しています");
        }
        /*
        last_polygon.outer().push_back(clicked);

        if(last_polygon.outer().size()>2){
            if(bg::equals( last_polygon.outer().at(0) , last_polygon.outer().at(last_polygon.outer().size()))){

                bg::correct(last_polygon);
            }
        }
        */
        this->update();
    }
}

void ManPowerProb::keyPressEvent(QKeyEvent *event){

    auto checkWithIn = [&]{
        for(auto piece : pieces){
            std::vector<polygon_i>intersection;
            bg::intersection(piece,last_polygon,intersection);
            for(auto poly : intersection){
                if(bg::area(poly)!=0)return true;
            }
        }
        return false;//重なってるならtrueを返す
    };

    if(event->key() == Qt::Key_A){//AでexportCSV

        std::vector<procon::NeoExpandedPolygon> neopieces;
        std::vector<procon::NeoExpandedPolygon> neoframes;
        int id=0;
        for(auto piece : pieces){
            procon::NeoExpandedPolygon neopiece(id);
            neopiece.resetPolygonForce(piece);
            neopieces.push_back(neopiece);
            ++id;
        }
        id=0;
        if(frames.size()){
            procon::NeoExpandedPolygon neoframe(id);
            neoframe.resetPolygonForce(not_put_part);
            neoframes.push_back(neoframe);
            field.setFrame(neoframes);
        }else{
            for(auto frame : frames){
                procon::NeoExpandedPolygon neoframe(id);
                neoframe.resetPolygonForce(frame);
                neoframes.push_back(neoframe);
                ++id;
            }
            field.setFrame(neoframes);
        }
        field.setElementaryPieces(neopieces);
        NeoPolygonIO::exportPolygon(this->field,"../../procon2017-comp/humanpowerfield.csv");
        logger->info("exported CSV");
    }

    if(event->key() == Qt::Key_L){//Lでポリゴンの生成
        if(last_polygon.outer().size() < 3)logger->error("始点と終点を結ぶことができません");
        else{
            last_polygon.outer().push_back(last_polygon.outer().at(0));
            bg::correct(last_polygon);
            if(!bg::is_valid(last_polygon) || checkWithIn())logger->error("ポリゴンが不自然な形になっています");
            else{
                if(is_frame)frames.push_back(last_polygon);
                else pieces.push_back(last_polygon);
                logger->info("polygonを生成しました");
            }
            last_polygon.clear();
        }
    }
    if(event->key() == Qt::Key_M){//Mでモード変更
        if(last_polygon.outer().size()!=0){
            logger->error("polygonの生成中のためモードを変更できません");
        }else{
            is_frame = !is_frame;
            logger->info(is_frame
                     ? "changed to frame_mode"
                     : "chenged to piece_mode");
        }
    }

    if(event->key() == Qt::Key_B){//Bで最も新しい頂点の削除
        if(last_polygon.outer().size()!=0){
            last_polygon.outer().pop_back();
            logger->info("頂点を削除しました");
        }else logger->error("頂点を削除できませんでした");
    }

    if(event->key() == Qt::Key_P){//Pで最も新しいピースの削除
        if(pieces.size()!=0){
            pieces.pop_back();
            logger->info("ピースを削除しました");
        }else logger->error("ピースを削除できませんでした");
    }

    if(event->key() == Qt::Key_F){//Fで最も新しいフレームの削除
        if(frames.size()!=0){
            frames.pop_back();
            logger->info("フレームを削除しました");
        }else logger->error("フレームを削除できませんでした");
    }

    this->update();
}

bool ManPowerProb::checkCanPlacePiece(point_i point){//pieceが置けるならtrueを返す
    if(frames.size()==0){//フレームが無い時
        if(!bg::intersects(not_put_part,point))return false;//枠外ならfalse
    }else{
        bool frame_flag=false;
        for(auto frame : frames){
            if(bg::intersects(frame,point)){
                frame_flag=true;
                for(auto poly_point : last_polygon.outer()){
                    if(!bg::intersects(frame,poly_point))return false;
                }
            }
        }
        if(!frame_flag)return false;//全ての枠の外ならfalse
    }
    for(auto piece : pieces){
        if(bg::within(point,piece))return false;//pieceと重なっているならfalse
    }
    return true;
}

bool ManPowerProb::checkCanPlaceFrame(point_i point){
    if(!bg::intersects(not_put_part,point))return false;
    for(auto frame : frames){
        if(bg::intersects(frame,point))return false;//frameと重なっているならfalse
    }
    for(auto piece : pieces){
        if(bg::intersects(piece,point))return false;//pieceと重なっているならfalse
    }
    return true;
}

QPointF ManPowerProb::translateToQPoint(point_i point){//point_iを上画面のgridと対応させるようにQPointFに変換する
    int pointx = point.x();
    int pointy = point.y();
    return QPointF(left_right_margin + pointx * grid_size, top_bottom_margin + pointy * grid_size);
}

point_i ManPowerProb::translateToPointi(QPointF point){//point_iを上画面のgridと対応させるようにQPointFに変換する
    //QPointF→point_iへの変換

    int pointx = point.x() - left_right_margin;
    int pointy = point.y() - top_bottom_margin;

    point_i clickedpos( pointx / grid_size, pointy / grid_size);
    std::cout << bg::dsv(clickedpos) << std::endl;
    return clickedpos;
}
