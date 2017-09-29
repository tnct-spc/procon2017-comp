#include "imagerecongnitionwithhumanpower.h"
#include "ui_imagerecongnitionwithhumanpower.h"
#include "precompile.h"
#include "polygonviewer.h"
#include <QLayout>

imagerecongnitionwithhumanpower::imagerecongnitionwithhumanpower(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::imagerecongnitionwithhumanpower)
{
    ui->setupUi(this);
//    edited_button = new QPushButton();
//    edited_button->setText("edited");
//    connect(edited_button,&QPushButton::clicked,this,&imagerecongnitionwithhumanpower::clickedEditedButton);
//    ui->verticalLayout->addWidget(edited_button);
}

imagerecongnitionwithhumanpower::~imagerecongnitionwithhumanpower()
{
    delete ui;
}

void imagerecongnitionwithhumanpower::setPolygon(polygon_t polygon){
    this->polygon = polygon;
    for(point_t i : polygon.outer()){
        QPointF point(i.x(),i.y());
        this->points.push_back(point);
    }
    this->update();
}

void imagerecongnitionwithhumanpower::clickedEditedButton(){

}
QPointF imagerecongnitionwithhumanpower::toCoordinate(double window_x, double window_y){
    const int N = 10;//小数点第Nまでの四捨五入
    QPointF buf((window_x - left_right_margin)/grid_size,
                  (window_y - top_buttom_margin)/grid_size);
    QPointF point(std::round(buf.x()*N) / N , std::round(buf.y()*N) / N);
    return point;
}

void imagerecongnitionwithhumanpower::mousePressEvent(QMouseEvent *event){
    QPointF selected_point = toCoordinate(event->x(),event->y());
    auto itr = std::find(points.begin(),points.end(),selected_point);
    if(itr == points.end()) return;

    std::cout<<"hello"<<std::endl;
}

void imagerecongnitionwithhumanpower::mouseReleaseEvent(QMouseEvent *event){
    QPointF point = toCoordinate(event->x(),event->y());

}

void imagerecongnitionwithhumanpower::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    const int grid_margin = 4;

    int window_width = this->width();
    int window_height = this->height();

    painter.setBrush(QBrush(QColor("#E5E6DB")));
    painter.drawRect(0,0,window_width,window_height);

    std::vector<QPointF> points;
    for(unsigned int a = 0; a < polygon.outer().size(); a++){
        points.push_back(QPointF(polygon.outer()[a].x(),polygon.outer()[a].y()));
    }

    auto minmaxX = std::minmax_element(points.begin(),points.end(), [](QPointF a,QPointF b){ return a.x() < b.x(); });
    auto minmaxY = std::minmax_element(points.begin(),points.end(), [](QPointF a,QPointF b){ return a.y() < b.y(); });

    int grid_col = std::ceil(minmaxX.second->x() - minmaxX.first->x());
    int grid_row = std::ceil(minmaxY.second->y() - minmaxY.first->y());

    grid_size =
                    window_width <= window_height
                    ? window_width / (grid_col + grid_margin)
                    : window_height / (grid_row + grid_margin);
    top_buttom_margin = (window_height - grid_size * grid_row) / 2;
    left_right_margin = (window_width - grid_size * grid_col) / 2;

    painter.setBrush(QBrush(QColor("#00FFFF")));

    std::vector<QPointF> polygon_points;
    for(unsigned int a = 0;a < this->polygon.outer().size(); a++){
        int x_buf = grid_size * (polygon.outer()[a].x() - std::floor(minmaxX.first->x())) + left_right_margin;
        int y_buf = grid_size * (polygon.outer()[a].y() - std::floor(minmaxY.first->y())) + top_buttom_margin;
        polygon_points.push_back(QPointF(x_buf,y_buf));
    }
    painter.setPen(QPen(QColor("#99CC00")));
    painter.drawPolygon(&polygon_points.front(),polygon_points.size());

    painter.setPen(QPen(QColor("#F15A22")));
    QFont font;
    font.setPixelSize(20);
    painter.setFont(font);
    for(unsigned int point_index = 0; point_index < polygon_points.size() ; ++point_index){
        QString str;
        str += QString::number(point_index);
        str += "(";
        str += QString::number(polygon.outer()[point_index].x());
        str += ",";
        str += QString::number(polygon.outer()[point_index].y());
        str += ")";
        painter.drawText(polygon_points[point_index] ,str);
    }


    painter.setPen(QPen(QColor("#000000")));
    for (int current_col = 0; current_col < grid_col + 1; ++current_col) {
        int x = current_col * grid_size + left_right_margin;
        painter.drawLine(x,top_buttom_margin,x,window_height - top_buttom_margin);
    }
    for (int current_row = 0; current_row < grid_row + 1; ++current_row) {
        int y = current_row * grid_size + top_buttom_margin;
        painter.drawLine(left_right_margin,y,window_width - left_right_margin,y);
    }

}
