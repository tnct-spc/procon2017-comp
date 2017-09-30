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


    QGraphicsView *view = new QGraphicsView();
    QGraphicsScene *scene = new QGraphicsScene();
    view->resize(600,400);
    std::cout << "w :" << view->width() << " h :" << view->height() << std::endl;
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

    connect(ui->center_x,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[&](int val){ManProb->centerx=val;});
    connect(ui->center_y,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[&](int val){ManProb->centery=val;});

    connect(ui->zoom,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),[&](double val){ManProb->zoom=val;});

    connect(ui->add_point,&QPushButton::clicked,ManProb,[&]{
        ManProb->addPoint(point_i(spin_x,spin_y));
        ManProb->update();
    });
}

ManPowerGui::~ManPowerGui()
{
    delete ui;
}

