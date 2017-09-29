#include "neosinglepolygondisplay.h"
#include "ui_neosinglepolygondisplay.h"
#include "neoexpandedpolygon.h"

#include <vector>
#include <QPainter>

NeoSinglePolygonDisplay::NeoSinglePolygonDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NeoSinglePolygonDisplay)
{
    ui->setupUi(this);
}

NeoSinglePolygonDisplay::~NeoSinglePolygonDisplay()
{
    delete ui;
}

//std::unique_ptr<NeoSinglePolygonDisplay> createInstance(procon::NeoExpandedPolygon const& print_polygon,std::string window_name);
std::unique_ptr<NeoSinglePolygonDisplay> NeoSinglePolygonDisplay::createInstance(polygon_i const& print_polygon,std::string window_name,bool enlarge_polygogn)
{
    std::unique_ptr<NeoSinglePolygonDisplay> instance(new NeoSinglePolygonDisplay());
    instance->setPolygon(print_polygon);
    instance->setWindowTitle(QString::fromStdString(window_name));
    instance->setIsEnlargedPolygon(enlarge_polygogn);

    return std::move(instance);
}

void NeoSinglePolygonDisplay::setPolygon(polygon_i _polygon)
{
    this->polygon = _polygon;
}

void NeoSinglePolygonDisplay::setIsEnlargedPolygon(bool is_enlarged_polygon)
{
    this->enlarged_polygon = is_enlarged_polygon;
}

void NeoSinglePolygonDisplay::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    const int grid_margin = 4;

    int window_width = this->width();
    int window_height = this->height();

    painter.setBrush(QBrush(QColor("#E5E6DB")));
    painter.drawRect(0,0,this->width(),this->height());

    //push_back points to std::vector<QPoint>
    std::vector<QPoint> points;
    for(unsigned int a = 0; a < polygon.outer().size(); a++){
        points.push_back(QPoint(polygon.outer()[a].x(),polygon.outer()[a].y()));
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

    auto getRadian = [&](){
       procon::NeoExpandedPolygon expandedpolygon;
       expandedpolygon.resetPolygonForce(polygon);
       std::vector<double> angles = expandedpolygon.getSideAngle_degree();
    };

    painter.setBrush(QBrush(QColor("#00FFFF")));

    std::vector<QPoint> polygon_points;
    for(unsigned int a = 0;a < this->polygon.outer().size(); a++){
        int x_buf = enlarged_polygon ? grid_size * polygon.outer()[a].x() + left_right_margin : grid_size * (polygon.outer()[a].x() - minmaxX.second->x()) + left_right_margin;
        int y_buf = enlarged_polygon ? grid_size * polygon.outer()[a].y() + top_buttom_margin : grid_size * (polygon.outer()[a].y() - minmaxY.second->y()) + top_buttom_margin;
        polygon_points.push_back(QPoint(x_buf,y_buf));
    }
    painter.setPen(QPen(QColor("#99CC00")));
    painter.drawPolygon(&polygon_points.front(),polygon_points.size());

    painter.setPen(QPen(QColor("#F15A22")));
    QFont font;
    font.setPixelSize(20);
    painter.setFont(font);
    QString str;
    for(unsigned int point_index = 0; point_index < polygon_points.size() - 1; ++point_index){
        str += QString::number(point_index);
        str += "(";
        str += QString::number(polygon_points[point_index].x());
        str += ",";
        str += QString::number(polygon_points[point_index].y());
        str += ")";
        //str +=
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

    getRadian();
}
