#include "imagerecongnitionwithhumanpower.h"
#include "ui_imagerecongnitionwithhumanpower.h"

imagerecongnitionwithhumanpower::imagerecongnitionwithhumanpower(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::imagerecongnitionwithhumanpower)
{
    ui->setupUi(this);

    my_graphics_view = new MyGraphicsView();
    ui->horizontalLayout->addWidget(my_graphics_view);
    connect(ui->enlargement,&QPushButton::clicked,this,&imagerecongnitionwithhumanpower::clickedEnlargement);
    connect(ui->reduction,&QPushButton::clicked,this,&imagerecongnitionwithhumanpower::clickedReduction);
}

void imagerecongnitionwithhumanpower::clickedEnlargement(){
}

void imagerecongnitionwithhumanpower::clickedReduction(){
}

imagerecongnitionwithhumanpower::~imagerecongnitionwithhumanpower()
{
    delete ui;
}

void imagerecongnitionwithhumanpower::setPolygon(polygon_t polygon){
    this->polygon = polygon;
    my_graphics_view->setPolygon(polygon);
}
