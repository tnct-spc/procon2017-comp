#include "mygraphicsview.h"
#include "ui_mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyGraphicsView)
{
    ui->setupUi(this);
}

MyGraphicsView::~MyGraphicsView()
{
    delete ui;
}

void MyGraphicsView::setPolygon(polygon_t const& polygon_){
    polygon = polygon_;
    bg::correct(polygon);

    points.clear();
    for(unsigned int i = 0 ; i<polygon.outer().size() - 1;i++){
        points.push_back(QPointF(polygon.outer()[i].x() * magnification , polygon.outer()[i].y() * magnification));
    }

    centor_points.clear();
    for(unsigned int i = 0 ; i<polygon.outer().size() -1 ; i++){
        int next_index = i+1 == polygon.outer().size() ? 0 : i+1;
        centor_points.push_back(QPointF(((polygon.outer()[i].x() + polygon.outer()[next_index].x()) / 2) * magnification ,
                                        ((polygon.outer()[i].y() + polygon.outer()[next_index].y()) / 2) * magnification));
    }
    this->update();
}

polygon_t MyGraphicsView::getPolygon(){
    return this->polygon;
}

void MyGraphicsView::polygonUpdate(){
    polygon_t changed_polygon;
    for(QPointF i : points){
        changed_polygon.outer().push_back(point_t(i.x() / magnification , i.y() / magnification));
    }
    setPolygon(changed_polygon);
}

void MyGraphicsView::setImage(cv::Mat const& image_){
    this->image = image_;
    this->update();
}

QPointF MyGraphicsView::toPolygonPoint(double window_x, double window_y){
//    const int N = 5;//小数点第Nまでを取る
//    double N_10 = std::pow(10,N);
    QPointF buf((window_x - left_right_margin - std::floor(minXY.first))/grid_size,
                  (window_y - top_buttom_margin - std::floor(minXY.second))/grid_size);
//    QPointF point(std::round(buf.x()*N_10) / N_10 , std::round(buf.y()*N_10) / N_10);
//    return point;
    return buf;
}

QPoint MyGraphicsView::toWindowPoint(QPointF point){
    int x_buf = grid_size * point.x() - std::floor(minXY.first) + left_right_margin;
    int y_buf = grid_size * point.y() - std::floor(minXY.second) + top_buttom_margin;
    return QPoint(x_buf,y_buf);
}

int MyGraphicsView::isInTolerance(QPointF point){
    auto itr = std::find_if(points.begin(),points.end(),[&](QPointF p){
        return (std::fabs(p.x() - point.x()) <= threshold) && (std::fabs(p.y() - point.y()) <= threshold);
    });
    return itr == points.end() ? -1 : std::distance(points.begin(),itr);
}

void MyGraphicsView::removePoint(int index)
{
    if(points.size() == 3) return;
    points.erase(points.begin() + index);
    polygonUpdate();
}

void MyGraphicsView::insertPoint(int x, int y){
    QPointF selected_point = toPolygonPoint(x,y);

    auto itr = std::find_if(centor_points.begin(),centor_points.end(),[&](QPointF p){
        return (std::fabs(p.x() - selected_point.x()) <= threshold) && (std::fabs(p.y() - selected_point.y()) <= threshold);
    });

    int insert_index = std::distance(centor_points.begin(),itr);
    if(itr != centor_points.end()){
        points.insert(points.begin() + insert_index + 1 , selected_point);
        polygonUpdate();
    }
}

void MyGraphicsView::mousePressEvent(QMouseEvent *event){
    int is_in_tolerance = isInTolerance(toPolygonPoint(event->x(),event->y()));
    selecting = is_in_tolerance != -1;
    is_point_retouch_mode = false;
    if(event->button() == Qt::MouseButton::LeftButton){
        //pointの移動(releaseとセット)
        if(selecting){
            is_point_retouch_mode = true;
            move_index = is_in_tolerance;
        }
    }else if(event->button() == Qt::MouseButton::RightButton){
        //pointの追加、消去
        if(is_in_tolerance == -1) insertPoint(event->x(),event->y());
        else removePoint(is_in_tolerance);
    }
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event){
    if(is_point_retouch_mode){
        points[move_index] = toPolygonPoint(event->x(),event->y());
        polygonUpdate();
    }
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event){
    selecting = false;
    if(is_point_retouch_mode){
        is_point_retouch_mode = false;
        points[move_index] = toPolygonPoint(event->x(),event->y());
        polygonUpdate();
    }
}

void MyGraphicsView::paintEvent(QPaintEvent *event)
{
    int window_width = this->width();
    int window_height = this->height();

    const int grid_margin = 2;

    auto minmaxX = std::minmax_element(points.begin(),points.end(), [](QPointF a,QPointF b){ return a.x() < b.x(); });
    auto minmaxY = std::minmax_element(points.begin(),points.end(), [](QPointF a,QPointF b){ return a.y() < b.y(); });

    minXY.first = minmaxX.first->x();
    minXY.second = minmaxY.first->y();

    int grid_col = std::ceil(minmaxX.second->x()) - std::floor(minmaxX.first->x());
    int grid_row = std::ceil(minmaxY.second->y()) - std::floor(minmaxY.first->y());

    grid_size =window_width <= window_height
                    ? window_width / (grid_col + grid_margin)
                    : window_height / (grid_row + grid_margin);
    top_buttom_margin = (window_height - grid_size * grid_row) / 2;
    left_right_margin = (window_width - grid_size * grid_col) / 2;

//    this->setMinimumSize(minmaxX.first->x(),minmaxY.first->y());

    QPainter painter(this);

    //画像表示
    cv::Mat buf_mat;
    cv::cvtColor(image, buf_mat, CV_RGB2BGR);
    QImage qimage((uchar *)buf_mat.data, buf_mat.cols, buf_mat.rows,(int)buf_mat.step, QImage::Format_RGB888);

    double image_window_width = qimage.width() * magnification;
    double image_window_height = qimage.height() * magnification;

    QRect image_rect(QPoint(left_right_margin,top_buttom_margin),
                            toWindowPoint(QPointF(image_window_width , image_window_height)
                     ));
    painter.drawImage(image_rect,qimage);

    this->resize(image_window_width / magnification,image_window_height / magnification);

    //polygon表示
    QVector<QPoint> window_points;
    for(QPointF i : points){
        window_points.push_back(toWindowPoint(i));
    }
    painter.setBrush(QBrush(QColor("#5500FFFF")));
    painter.setPen(QPen(QColor("#FFFFFF00")));
    window_points.push_back(toWindowPoint(points.at(0)));
    painter.drawPolyline(&window_points.front(),window_points.size());
    window_points.erase(window_points.end()-1);

    //point表示
    painter.setPen(QPen(QColor("#000000")));
    painter.setBrush(QColor("#55FFFFFF"));
    QRect point_rect;
    for(QPoint i : window_points){
        point_rect = QRect(i.x() - (threshold * grid_size) / 2 , i.y() - (threshold * grid_size) / 2 ,
                           threshold * grid_size,threshold * grid_size);
        painter.drawRect(point_rect);
    }

    //point中点を表示
    QVector<QPoint> window_centor_points;
    for(QPointF i : centor_points) window_centor_points.push_back(toWindowPoint(i));
    painter.setPen(QPen(QColor("#FFFFFF")));
    painter.setBrush(QColor("#55000000"));
    for(QPoint i : window_centor_points){
        point_rect = QRect(i.x() - (threshold * grid_size) / 2 , i.y() - (threshold * grid_size) / 2 ,
                           threshold * grid_size,threshold * grid_size);
        painter.drawRect(point_rect);
    }

    //index表示
    painter.setPen(QPen(QColor("#F15A22")));
    QFont font;
    font.setPixelSize(20);
    painter.setFont(font);
    for(int point_index = 0; point_index < window_points.size() ; ++point_index){
        QString str;
        str += QString::number(point_index);
//        str += "(";
//        str += QString::number(points[point_index].x());
//        str += ",";
//        str += QString::number(points[point_index].y());
//        str += ")";
        painter.drawText(window_points[point_index],str);
    }

//    //grid表示
//    painter.setPen(QPen(QColor("#000000")));
//    for (int current_col = 0; current_col < grid_col + 1; ++current_col) {
//        int x = current_col * grid_size + left_right_margin;
//        painter.drawLine(x,top_buttom_margin,x,window_height - top_buttom_margin);
//    }
//    for (int current_row = 0; current_row < grid_row + 1; ++current_row) {
//        int y = current_row * grid_size + top_buttom_margin;
//        painter.drawLine(left_right_margin,y,window_width - left_right_margin,y);
//    }
}

