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
    viewer->setFixedSize(200, 200);

   viewer->setPolygon(poly);
   viewer->setIsEnlargedPolygon(false);

   const int lineup = 1;
    this->ui->container->addWidget(viewer,(polygons.size()-1)/lineup,(polygons.size()-1)%lineup);
}
