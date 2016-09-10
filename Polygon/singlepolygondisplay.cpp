#include "singlepolygondisplay.h"
#include "ui_singlepolygondisplay.h"

#include <QPainter>
#include <QPen>

SinglePolygonDisplay::SinglePolygonDisplay(QWidget *parent = 0) :
    QWidget(parent),
    ui(new Ui::SinglePolygonDisplay)
{
    ui->setupUi(this);
}

std::unique_ptr<SinglePolygonDisplay> SinglePolygonDisplay::create(procon::ExpandedPolygon& polygon, int scale, std::string wname_)
{
    std::unique_ptr<SinglePolygonDisplay> instance(new SinglePolygonDisplay());
    instance->polygon = polygon;
    instance->scale = scale;
    QString wname = QString::fromStdString(wname_);
    instance->setWindowTitle(wname);
    return std::move(instance);
}

SinglePolygonDisplay::~SinglePolygonDisplay()
{
    delete ui;
}

void SinglePolygonDisplay::paintEvent(QPaintEvent *)
{
    int height = this->height();
    int width = this->width();
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 3));

    auto drawPolygon = [&](std::vector<QPointF> points){
        int size = points.size();
        QPointF* draw_point = new QPointF[points.size()];
        static const int max = scale;
        static const int margin = 20;
        int y = height - margin;
        int x = height - margin;
        int y_margin = margin/2;
        int x_margin = (width-height)/2 + margin/2;
        for(int i=0;i<size;i++){
            draw_point[i].setX(((points.at(i).x())*x/max)+(x/2)+x_margin);
            draw_point[i].setY(((points.at(i).y())*y/max)+(y/2)+y_margin);
        }
        painter.drawPolygon(draw_point,size);
        delete[] draw_point;
    };

    //draw piece
    int piece_size = polygon.getPolygon().outer().size()-1;
    std::vector<QPointF> points;
    for(int i=0;i<piece_size;i++){
        points.push_back(QPointF(polygon.getPolygon().outer()[i].x(),polygon.getPolygon().outer()[i].y()));
    }
    painter.setBrush(QBrush(QColor("#0f5ca0")));
    drawPolygon(points);

}
