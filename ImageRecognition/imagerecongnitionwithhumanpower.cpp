#include "imagerecongnitionwithhumanpower.h"
#include "ui_imagerecongnitionwithhumanpower.h"

imagerecongnitionwithhumanpower::imagerecongnitionwithhumanpower(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::imagerecongnitionwithhumanpower)
{
    ui->setupUi(this);

    view = new QGraphicsView();
    scene = new QGraphicsScene();
    my_graphics_view = new MyGraphicsView();

    scene->addWidget(my_graphics_view);
    view->setScene(scene);

    ui->horizontalLayout->addWidget(view);
    view->resize(my_graphics_view->width() , my_graphics_view->height());

    connect(ui->editedButton,&QPushButton::clicked,this,&imagerecongnitionwithhumanpower::clickedEditedButton);
}

void imagerecongnitionwithhumanpower::clickedEditedButton(){
    emit returnPolygon(my_graphics_view->getPolygon());
//    this->close();
}

imagerecongnitionwithhumanpower::~imagerecongnitionwithhumanpower()
{
    delete ui;
}

void imagerecongnitionwithhumanpower::setPolygon(polygon_t const& polygon){
    my_graphics_view->setPolygon(polygon);
}

void imagerecongnitionwithhumanpower::setImage(cv::Mat const& image){
    my_graphics_view->setImage(image);
}
