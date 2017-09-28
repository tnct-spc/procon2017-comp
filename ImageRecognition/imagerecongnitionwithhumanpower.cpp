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

//    ui->verticalLayout->addWidget(edited_button);
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
    scale = 10;
    int window_height = this->height();
    int window_width = this->width();

    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 3));

    std::vector<QPointF> points;
    for(int i=0;i<polygon.getSize();i++) points.push_back(QPointF(polygon.getPolygon().outer()[i].x(),polygon.getPolygon().outer()[i].y()));
    double x_max,y_max,x_min,y_min;
    int x_max_i,y_max_i,x_min_i,y_min_i;
    x_max = (std::max_element(points.begin(), points.end(), [](QPointF a, QPointF b){return a.x() > b.x();}))->x();
    y_max = (std::max_element(points.begin(), points.end(), [](QPointF a, QPointF b){return a.y() > b.y();}))->y();
    x_min = (std::min_element(points.begin(), points.end(), [](QPointF a, QPointF b){return a.x() < b.x();}))->x();
    y_min = (std::min_element(points.begin(), points.end(), [](QPointF a, QPointF b){return a.y() < b.y();}))->y();
    x_max_i = ceil(x_max);
    y_max_i = ceil(y_max);
    x_min_i = floor(x_min);
    y_min_i = floor(y_min);

    auto drawPolygon = [&](){
        int grid_margin  = 10;
        bool enlarged_polygon = false;

        int grid_col = enlarged_polygon ? 101 : x_max - x_min;
        int grid_row = enlarged_polygon ? 65 : y_max - y_min;

        const int grid_size =
                        window_width <= window_height
                        ? window_width / (grid_col + grid_margin)
                        : window_height / (grid_row + grid_margin);
        const int top_buttom_margin = (window_height - grid_size * grid_row) / 2;
        const int left_right_margin = (window_width - grid_size * grid_col) / 2;

        std::vector<QPointF> polygon_points;
        for(unsigned int a = 0;a < this->polygont.outer().size(); a++){
            int x_buf = enlarged_polygon ? grid_size * polygont.outer()[a].x() + left_right_margin : grid_size * (polygont.outer()[a].x() - x_max + left_right_margin);
            int y_buf = enlarged_polygon ? grid_size * polygont.outer()[a].y() + top_buttom_margin : grid_size * (polygont.outer()[a].y() - y_max + top_buttom_margin);
            polygon_points.push_back(QPoint(x_buf,y_buf));
        }
        painter.setPen(QPen(QColor("#99CC00")));
        painter.drawPolygon(&polygon_points.front(),polygon_points.size());
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
    painter.setBrush(QBrush(QColor("#0f5ca0")));
    drawPolygon();
    drawGrid();
}
