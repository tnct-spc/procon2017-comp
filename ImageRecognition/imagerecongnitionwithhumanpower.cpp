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

    ui->forMyWidgetLayout->addWidget(view);
    view->resize(my_graphics_view->width() , my_graphics_view->height());

    connect(ui->editedButton,&QPushButton::clicked,this,&imagerecongnitionwithhumanpower::clickedEditedButton);
    connect(ui->enlargementButton,&QPushButton::clicked,this,&imagerecongnitionwithhumanpower::clickedEnlargementButton);
    connect(ui->reductionButton,&QPushButton::clicked,this,&imagerecongnitionwithhumanpower::clickedReductionButton);
}

void imagerecongnitionwithhumanpower::clickedEditedButton(){
    auto polygonIO = [](std::string path , polygon_t polygon){
        std::ofstream output(path);
        for(point_t i : polygon.outer()){
            output << bg::dsv(i) << std::endl;
        }
        output.close();
    };

    polygonIO("../../procon2017-comp/CSV/edited_polygon.csv",my_graphics_view->getPolygon());
    emit returnPolygon(my_graphics_view->getPolygon());
//    this->close();
}

void imagerecongnitionwithhumanpower::clickedEnlargementButton(){
    view->scale(1.1,1.1);
}

void imagerecongnitionwithhumanpower::clickedReductionButton(){
    view->scale(0.9,0.9);
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
