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
}

ManPowerProb::~ManPowerProb()
{
    delete ui;
}

void ManPowerProb::paintEvent(QPaintEvent *event){

    QPainter painter(this);
    const QString back_ground_color = "#EEEEBB";
    const QString frame_color = "#EEEEEE";
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
        painter.setPen(QPen(QBrush(Qt::black),0.5));
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
        painter.setPen(QPen(QBrush(Qt::red),2.0));
        if(draw_points.size()>1){
            for(unsigned int count=0;count<draw_points.size() - 1;++count){
                painter.drawLine(draw_points.at(count),draw_points.at(count + 1));
            }
        }
        draw_points.clear();
    };

    auto drawFrames = [&]{
        painter.setPen(QPen(QBrush(Qt::black),2.0));
        painter.setBrush(QBrush(QColor(frame_color)));
        QVector<QPointF> draw_points;
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


        if(checkCanPlace(clicked))logger->error("範囲外を指定しています");
        else{
            last_polygon.outer().push_back(clicked);
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

    if(event->key() == Qt::Key_A){//EnterでexportCSV

        std::vector<procon::NeoExpandedPolygon> neopieces;
        std::vector<procon::NeoExpandedPolygon> neoframes;

        procon::NeoExpandedPolygon neopiece;
        for(auto piece : pieces){
            neopiece.resetPolygonForce(piece);
            neopieces.push_back(neopiece);
        }
        procon::NeoExpandedPolygon neoframe;
        for(auto frame : frames){
            neoframe.resetPolygonForce(frame);
            neoframes.push_back(neoframe);
        }
        field.setFrame(neoframes);
        field.setElementaryPieces(neopieces);
        NeoPolygonIO::exportPolygon(this->field,"../../procon2017-comp/humanpowerfield.csv");
        logger->info("exported CSV");
    }

    if(event->key() == Qt::Key_L){
        if(last_polygon.outer().size() < 3)logger->error("始点と終点を結ぶことができません");
        else{
            last_polygon.outer().push_back(last_polygon.outer().at(0));
            bg::correct(last_polygon);
            if(!bg::is_valid(last_polygon))logger->error("ポリゴンが不自然な形になっています");
            else{
                if(is_frame)frames.push_back(last_polygon);
                else pieces.push_back(last_polygon);
                logger->info("polygonを生成しました");
            }
            last_polygon.clear();
        }
    }
    if(event->key() == Qt::Key_M){
        is_frame = !is_frame;
        logger->info(is_frame
                     ? "changed to frame_mode"
                     : "chenged to piece_mode");
    }
    this->update();
}

bool ManPowerProb::checkCanPlace(polygon_i poly){
    if(frames.size()){//フレームが無い時

        polygon_i not_put_part;
        not_put_part.outer().push_back(point_i(0,0));
        not_put_part.outer().push_back(point_i(101,0));
        not_put_part.outer().push_back(point_i(101,65));
        not_put_part.outer().push_back(point_i(0,65));
        not_put_part.outer().push_back(point_i(0,0));
        bg::correct(not_put_part);
        if(!bg::intersects(not_put_part,poly))return false;
    }else{
        bool frame_flag=false;
        for(auto frame : frames){
            if(bg::intersects(frame,poly))frame_flag=true;
        }
        if(!frame_flag)return false;
    }
    for(auto piece : pieces){
        if(bg::intersects(piece,poly))return false;
    }
    return true;
}

bool ManPowerProb::checkCanPlace(point_i point){
    if(frames.size()){//フレームが無い時

        polygon_i not_put_part;
        not_put_part.outer().push_back(point_i(0,0));
        not_put_part.outer().push_back(point_i(101,0));
        not_put_part.outer().push_back(point_i(101,65));
        not_put_part.outer().push_back(point_i(0,65));
        not_put_part.outer().push_back(point_i(0,0));
        bg::correct(not_put_part);
        if(!bg::intersects(not_put_part,point))return false;
    }else{
        bool frame_flag=false;
        for(auto frame : frames){
            if(bg::intersects(frame,point))frame_flag=true;
        }
        if(!frame_flag)return false;
    }
    for(auto piece : pieces){
        if(bg::intersects(piece,point))return false;
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
