#include "manpowergui.h"
#include "manpowerprob.h"
#include "ui_manpowergui.h"
#include <qboxlayout.h>



ManPowerGui::ManPowerGui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManPowerGui)
{
    ui->setupUi(this);
    ManProb = new ManPowerProb();
    ManProb->show();


    view = new QGraphicsView();
    scene = new QGraphicsScene();

    view->resize(600,400);
    ManProb->resize(view->width()-10,view->height()-10);
    scene->addWidget(ManProb);
    view->setScene(scene);
    view->show();

    connect(ui->change_mode,&QPushButton::clicked,ManProb,&ManPowerProb::changeMode);
    connect(ui->delete_piece,&QPushButton::clicked,ManProb,&ManPowerProb::deletePiece);
    connect(ui->delete_frame,&QPushButton::clicked,ManProb,&ManPowerProb::deleteFrame);
    connect(ui->delete_point,&QPushButton::clicked,ManProb,&ManPowerProb::deletePoint);

    connect(ui->create_polygon,&QPushButton::clicked,ManProb,&ManPowerProb::createPolygon);

    connect(ui->set_point_x,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[&](int val){spin_x=val;});
    connect(ui->set_point_y,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[&](int val){spin_y=val;});

    connect(ui->zoom,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),[&](double val){zoomIn(val);});

    connect(ui->add_point,&QPushButton::clicked,ManProb,[&]{
        ManProb->addPoint(point_i(spin_x,spin_y));
        ManProb->update();
    });

    connect(ManProb,SIGNAL(zoom_In()),this,SLOT(_zoom_In()));
    connect(ManProb,SIGNAL(zoom_Out()),this,SLOT(_zoom_Out()));
}

ManPowerGui::~ManPowerGui()
{
    delete ui;
}

void ManPowerGui::zoomIn(double value){
    if(!(value>99 || value<0.5)){
        double zoom_ = value / zoom;
        zoom = value;
        view->scale(zoom_,zoom_);
        view->update();
    }
}

void ManPowerGui::_zoom_In(){zoomIn(zoom+0.5);}
void ManPowerGui::_zoom_Out(){zoomIn(zoom-0.5);}

void ManPowerGui::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Z){
        zoomIn(zoom+0.5);
    }
    if(event->key() == Qt::Key_X){
        zoomIn(zoom-0.5);
    }
}
