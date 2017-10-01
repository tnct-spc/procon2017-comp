#include "singlepolygondock.h"
#include "ui_singlepolygondock.h"

SinglePolygonDock::SinglePolygonDock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SinglePolygonDock)
{
    ui->setupUi(this);

}

SinglePolygonDock::~SinglePolygonDock()
{
    delete ui;
}

void SinglePolygonDock::addPolygon(const polygon_i poly){
    polygons.push_back(poly);
    NeoSinglePolygonDisplay* viewer = new NeoSinglePolygonDisplay;
    viewer->setFixedSize(300, 300);

   viewer->setPolygon(poly);

    this->ui->container->addWidget(viewer,(polygons.size()-1)/3,(polygons.size()-1)%3);
}
