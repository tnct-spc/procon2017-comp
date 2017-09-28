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

    ui->verticalLayout->addWidget(edited_button);
}

imagerecongnitionwithhumanpower::~imagerecongnitionwithhumanpower()
{
    delete ui;
}

void imagerecongnitionwithhumanpower::setPolygon(polygon_t polygon){
    this->polygon = polygon;
}

void imagerecongnitionwithhumanpower::clickedEditedButton(){

}
