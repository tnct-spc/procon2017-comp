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

void MyGraphicsView::setPolygon(polygon_t polygon){
    bg::correct(polygon);
    this->polygon = polygon;
    points.clear();
    for(int i = 0 ; i<polygon.outer().size() - 1;i++){
        QPointF point(polygon.outer()[i].x() * magnification , polygon.outer()[i].y() * magnification);
        points.push_back(point);
    }
    this->update();
}

void MyGraphicsView::setImage(cv::Mat image){
    this->image = image;
}

QPointF MyGraphicsView::toPolygonPoint(double window_x, double window_y){
    const int N = 5;//小数点第Nまでを取る
    double N_10 = std::pow(10,N);
    QPointF buf((window_x - left_right_margin - std::floor(minXY.first))/grid_size,
                  (window_y - top_buttom_margin - std::floor(minXY.second))/grid_size);
    QPointF point(std::round(buf.x()*N_10) / N_10 , std::round(buf.y()*N_10) / N_10);
    return point;
}

QPoint MyGraphicsView::toWindowPoint(QPointF point){
    int x_buf = grid_size * point.x() - std::floor(minXY.first) + left_right_margin;
    int y_buf = grid_size * point.y() - std::floor(minXY.second) + top_buttom_margin;
    return QPoint(x_buf,y_buf);
}

int MyGraphicsView::isInTolerance(QPointF point){
    std::cout<<point.x()<<","<<point.y()<<std::endl;
    auto itr = std::find_if(points.begin(),points.end(),[&](QPointF p){
        return (std::fabs(p.x() - point.x()) <= threshold) && (std::fabs(p.y() - point.y()) <= threshold);
    });
    return itr == points.end() ? -1 : std::distance(points.begin(),itr);
}

void MyGraphicsView::mousePressEvent(QMouseEvent *event){
    QPointF selected_point = toPolygonPoint(event->x(),event->y());
    int is_in_tolerance = isInTolerance(selected_point);
    selecting = is_in_tolerance != -1;
    if(selecting){
        move_index = is_in_tolerance;
    }
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event){
    if(!selecting) return;
    points[move_index] = toPolygonPoint(event->x(),event->y());
    polygon_t changed_polygon;
    for(QPointF i : points){
        changed_polygon.outer().push_back(point_t(i.x() / magnification , i.y() / magnification));
    }
    setPolygon(changed_polygon);
    this->update();
}

void MyGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPointF selected_point = toPolygonPoint(event->x(),event->y());
    int is_in_tolerance = isInTolerance(selected_point);
    bool is_on_point = is_in_tolerance != -1;
    if(is_on_point){
        points.erase(points.begin() + is_in_tolerance -1);
    }else{
        points.push_back(selected_point);
    }
    polygon_t changed_polygon;
    for(QPointF i : points){
        changed_polygon.outer().push_back(point_t(i.x() / magnification , i.y() / magnification));
    }
    setPolygon(changed_polygon);
    this->update();
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
    QRect image_rect(QPoint(left_right_margin,top_buttom_margin),
                            toWindowPoint(QPointF(
                                minmaxX.second->x() + photo_margin * magnification ,
                                minmaxY.second->y() + photo_margin * magnification
                            )
                     ));
    painter.drawImage(image_rect,qimage);

    //polygon表示
    QVector<QPoint> window_points;
    for(QPointF i : points){
        window_points.push_back(toWindowPoint(i));
    }
    painter.setBrush(QBrush(QColor("#5500FFFF")));
    painter.drawPolygon(&window_points.front(),window_points.size());

    //index表示
    painter.setPen(QPen(QColor("#F15A22")));
    QFont font;
    font.setPixelSize(20);
    painter.setFont(font);
    for(unsigned int point_index = 0; point_index < window_points.size() ; ++point_index){
        QString str;
        str += QString::number(point_index);
        str += "(";
        str += QString::number(points[point_index].x());
        str += ",";
        str += QString::number(points[point_index].y());
        str += ")";
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

