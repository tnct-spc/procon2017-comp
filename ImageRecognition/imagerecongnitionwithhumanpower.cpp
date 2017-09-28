#include "imagerecongnitionwithhumanpower.h"
#include "ui_imagerecongnitionwithhumanpower.h"
#include "precompile.h"
#include "polygonviewer.h"
#include <QLayout>

imagerecongnitionwithhumanpower::imagerecongnitionwithhumanpower(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::imagerecongnitionwithhumanpower)
{
    bg::exterior_ring(polygont) = boost::assign::list_of<point_t>(0, 0)(0, 3)(3, 3)(0, 0);
    polygon.resetPolygonForce(polygont);

    ui->setupUi(this);
    edited_button = new QPushButton();
    edited_button->setText("edited");

    connect(edited_button,&QPushButton::clicked,this,&imagerecongnitionwithhumanpower::clickedEditedButton);

    ui->verticalLayout->addWidget(edited_button);
}

imagerecongnitionwithhumanpower::~imagerecongnitionwithhumanpower()
{
    delete ui;
}

void imagerecongnitionwithhumanpower::setPolygon(polygon_t polygon){
    this->polygont = polygon;
    this->polygon.resetPolygonForce(polygon);
}

void imagerecongnitionwithhumanpower::clickedEditedButton(){

}

procon::ExpandedPolygon imagerecongnitionwithhumanpower::polygonAdapter(polygon_t polygon){
    procon::ExpandedPolygon expandedpolygon;
    expandedpolygon.resetPolygonForce(polygon);
    return expandedpolygon;
}

void imagerecongnitionwithhumanpower::paintEvent(QPaintEvent *)
{
    int window_height = this->height();
    int window_width = this->width();
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 3));

    double x_max,y_max,x_min,y_min;
    int x_max_i,y_max_i,x_min_i,y_min_i;
    auto drawPolygon = [&](std::vector<QPointF> points){
        static const double margin = 10;
        const int size = points.size();

        x_max = (std::max_element(points.begin(), points.end(), [](QPointF a, QPointF b){return a.x() > b.x();}))->x();
        y_max = (std::max_element(points.begin(), points.end(), [](QPointF a, QPointF b){return a.y() > b.y();}))->y();
        x_min = (std::min_element(points.begin(), points.end(), [](QPointF a, QPointF b){return a.x() < b.x();}))->x();
        y_min = (std::min_element(points.begin(), points.end(), [](QPointF a, QPointF b){return a.y() < b.y();}))->y();
        x_max_i = ceil(x_max);
        y_max_i = ceil(y_max);
        x_min_i = floor(x_min);
        y_min_i = floor(y_min);

        const double width  = x_max - x_min;
        const double height = y_max - y_min;
        const double max = scale != -1 ? scale : width < height ? height : width;
        const double y = window_height - margin;
        const double x = window_height - margin;
        const double y_margin = margin/2;
        const double x_margin = (window_width-window_height)/2 + margin/2;
        const double x_offset = - (x_min + (x_max - x_min)/2);
        const double y_offset = - (y_min + (y_max - y_min)/2);
        QPointF* draw_point = new QPointF[points.size()];
        for(int i=0;i<size;i++){
            draw_point[i].setX(((points.at(i).x() + x_offset)*x/max)+x/2+x_margin);
            draw_point[i].setY(((points.at(i).y() + y_offset)*y/max)+y/2+y_margin);
        }
        //draw polygo
        painter.drawPolygon(draw_point,size);
        //draw number
        painter.setPen(QPen(QColor("#00ff00")));
        QFont font = painter.font();
        font.setPointSize(std::abs(y/30));
        painter.setFont(font);
        for(int count=0; count<size;++count){
            painter.drawText(draw_point[count], QString::number(count));
        }

        for(int cnt = 0; cnt < (size - 1); cnt++){
            double dx = points[cnt].x() - points[cnt + 1].x();
            double dy = points[cnt].y() - points[cnt + 1].y();
            double distance = std::sqrt(dx * dx + dy * dy);

            double draw_dx = draw_point[cnt + 1].x() - draw_point[cnt].x();
            draw_dx = draw_dx / 2;
            double draw_dy = draw_point[cnt + 1].y() - draw_point[cnt].y();
            draw_dy = draw_dy / 2;

            QPointF drawing_point(draw_point[cnt].x() + draw_dx ,draw_point[cnt].y() + draw_dy);

            painter.drawText(drawing_point,QString::fromStdString(std::to_string( distance )));
        }
        delete[] draw_point;
    };

    auto drawGrid = [&](){
        int grid_size = 10 , top_buttom_margin = 10 , left_right_margin = 10;

        painter.setPen(QPen(QColor("#000000")));
        for (int current_col = x_min_i; current_col < x_max_i + 1; ++current_col) {
            int x = current_col * grid_size + left_right_margin;
            painter.drawLine(x,top_buttom_margin,x,window_height - top_buttom_margin);
        }
        for (int current_row = y_min_i; current_row < x_max_i + 1; ++current_row) {
            int y = current_row * grid_size + top_buttom_margin;
            painter.drawLine(left_right_margin,y,window_width - left_right_margin,y);
        }
    };

    //draw piece
    int piece_size = polygon.getSize();
    std::vector<QPointF> points;
    for(int i=0;i<piece_size;i++){
        points.push_back(QPointF(polygon.getPolygon().outer()[i].x(),polygon.getPolygon().outer()[i].y()));
    }
    painter.setBrush(QBrush(QColor("#0f5ca0")));
    drawPolygon(points);
    drawGrid();
}
