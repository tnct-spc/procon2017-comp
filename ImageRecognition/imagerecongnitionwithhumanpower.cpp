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
    edited_button = new QPushButton();
    edited_button->setText("edited");

    connect(edited_button,&QPushButton::clicked,this,&imagerecongnitionwithhumanpower::clickedEditedButton);

    single_polygon_display = SinglePolygonDisplay::create(polygonAdapter(polygon),"",1).get();
    ui->verticalLayout->addWidget(single_polygon_display);
    ui->verticalLayout->addWidget(edited_button);
}

imagerecongnitionwithhumanpower::~imagerecongnitionwithhumanpower()
{
    delete ui;
}

void imagerecongnitionwithhumanpower::setPolygon(polygon_t polygon){
    this->polygon = polygon;
    single_polygon_display = SinglePolygonDisplay::create(polygonAdapter(polygon),"",1).get();
}

void imagerecongnitionwithhumanpower::clickedEditedButton(){

}

procon::ExpandedPolygon imagerecongnitionwithhumanpower::polygonAdapter(polygon_t polygon){
    procon::ExpandedPolygon expandedpolygon;
    expandedpolygon.resetPolygonForce(polygon);
    return expandedpolygon;
}
