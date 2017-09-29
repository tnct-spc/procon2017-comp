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
}

void imagerecongnitionwithhumanpower::clickedEditedButton(){

}

void imagerecongnitionwithhumanpower::mousePressEvent(QMouseEvent *event){
    std::cout<<"mousepressevent"<<std::endl;
    std::cout<<event->x()<<","<<event->y()<<std::endl;
}

void imagerecongnitionwithhumanpower::mouseReleaseEvent(QMouseEvent *event){
    std::cout<<"mousereleaseevent"<<std::endl;
    std::cout<<event->x()<<","<<event->y()<<std::endl;
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

    int grid_col = minmaxX.second->x() - minmaxX.first->x();
    int grid_row = minmaxY.second->y() - minmaxY.first->y();

    const int grid_size =
                    window_width <= window_height
                    ? window_width / (grid_col + grid_margin)
                    : window_height / (grid_row + grid_margin);
    const int top_buttom_margin = (window_height - grid_size * grid_row) / 2;
    const int left_right_margin = (window_width - grid_size * grid_col) / 2;

    painter.setBrush(QBrush(QColor("#00FFFF")));

    std::vector<QPointF> polygon_points;
    for(unsigned int a = 0;a < this->polygon.outer().size(); a++){
        int x_buf = grid_size * (polygon.outer()[a].x() - minmaxX.first->x()) + left_right_margin;
        int y_buf = grid_size * (polygon.outer()[a].y() - minmaxY.first->y()) + top_buttom_margin;
        polygon_points.push_back(QPointF(x_buf,y_buf));
    }
    painter.setPen(QPen(QColor("#99CC00")));
    painter.drawPolygon(&polygon_points.front(),polygon_points.size());

    painter.setPen(QPen(QColor("#F15A22")));
    QFont font;
    font.setPixelSize(20);
    painter.setFont(font);
    for(unsigned int point_index = 0; point_index < polygon_points.size() - 1; ++point_index){
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
        grid_x.push_back(x);
    }
    for (int current_row = 0; current_row < grid_row + 1; ++current_row) {
        int y = current_row * grid_size + top_buttom_margin;
        painter.drawLine(left_right_margin,y,window_width - left_right_margin,y);
        grid_y.push_back(y);
    }

}
