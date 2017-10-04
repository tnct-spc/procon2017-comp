#include "imagerecongnitionwithhumanpower.h"
#include "ui_imagerecongnitionwithhumanpower.h"

imagerecongnitionwithhumanpower::imagerecongnitionwithhumanpower(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::imagerecongnitionwithhumanpower)
{
    ui->setupUi(this);

    my_graphics_view = new MyGraphicsView();
    ui->horizontalLayout->addWidget(my_graphics_view);
    connect(ui->editedButton,&QPushButton::clicked,this,&imagerecongnitionwithhumanpower::clickedEditedButton);
}

void imagerecongnitionwithhumanpower::clickedEditedButton(){
    this->close();
}

imagerecongnitionwithhumanpower::~imagerecongnitionwithhumanpower()
{
    delete ui;
}

void imagerecongnitionwithhumanpower::setPolygon(polygon_t polygon){
    my_graphics_view->setPolygon(polygon);
}

void imagerecongnitionwithhumanpower::setImage(cv::Mat image){
    my_graphics_view->setImage(image);
}
