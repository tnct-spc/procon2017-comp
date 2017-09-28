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
    bool enlarged_polygon = false;
    QPainter painter(this);

    const int grid_margin = 4;

    int window_width = this->width();
    int window_height = this->height();

    painter.setBrush(QBrush(QColor("#E5E6DB")));
    painter.drawRect(0,0,this->width(),this->height());

    //push_back points to std::vector<QPoint>
    std::vector<QPoint> points;
    for(unsigned int a = 0; a < polygont.outer().size(); a++){
        points.push_back(QPoint(polygont.outer()[a].x(),polygont.outer()[a].y()));
    }

    auto minmaxX = std::minmax_element(points.begin(),points.end(), [](QPoint a,QPoint b){ return a.x() > b.x(); });
    auto minmaxY = std::minmax_element(points.begin(),points.end(), [](QPoint a,QPoint b){ return a.y() > b.y(); });

    int grid_col = enlarged_polygon ? 101 : minmaxX.first->x() - minmaxX.second->x();
    int grid_row = enlarged_polygon ? 65 : minmaxY.first->y() - minmaxY.second->y();

    const int grid_size =
                    window_width <= window_height
                    ? window_width / (grid_col + grid_margin)
                    : window_height / (grid_row + grid_margin);
    const int top_buttom_margin = (window_height - grid_size * grid_row) / 2;
    const int left_right_margin = (window_width - grid_size * grid_col) / 2;



    painter.setBrush(QBrush(QColor("#00FFFF")));

    std::vector<QPoint> polygont_points;
    for(unsigned int a = 0;a < this->polygont.outer().size(); a++){
        int x_buf = enlarged_polygon ? grid_size * polygont.outer()[a].x() + left_right_margin : grid_size * (polygont.outer()[a].x() - minmaxX.second->x()) + left_right_margin;
        int y_buf = enlarged_polygon ? grid_size * polygont.outer()[a].y() + top_buttom_margin : grid_size * (polygont.outer()[a].y() - minmaxY.second->y()) + top_buttom_margin;
        polygont_points.push_back(QPoint(x_buf,y_buf));
    }
    painter.setPen(QPen(QColor("#99CC00")));
    painter.drawPolygon(&polygont_points.front(),polygont_points.size());

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
