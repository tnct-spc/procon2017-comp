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
    NeoPolygonViewer* viewer = new NeoPolygonViewer;
    viewer->setFixedSize(500, 500);

    viewer->getInstance().displayPolygon(poly,"Piece",true);
    this->ui->container->addWidget(viewer,(polygons.size()-1)/5,(polygons.size()-1)%5);
}
