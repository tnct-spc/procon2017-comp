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

    connect(ui->change_mode,&QPushButton::clicked,ManProb,&ManPowerProb::changeMode);
    connect(ui->delete_piece,&QPushButton::clicked,ManProb,&ManPowerProb::deletePiece);
    connect(ui->delete_frame,&QPushButton::clicked,ManProb,&ManPowerProb::deleteFrame);
    connect(ui->delete_point,&QPushButton::clicked,ManProb,&ManPowerProb::deletePoint);

    connect(ui->create_polygon,&QPushButton::clicked,ManProb,&ManPowerProb::createPolygon);

    connect(ui->set_point_x,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[&](int val){spin_x=val; std::cout << spin_x << std::endl;});
    connect(ui->set_point_y,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[&](int val){spin_y=val; std::cout << spin_y << std::endl;});

    connect(ui->center_x,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[&](int val){ManProb->centerx=val; std::cout << ManProb->centerx << std::endl;});
    connect(ui->center_y,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[&](int val){ManProb->centery=val; std::cout << ManProb->centery << std::endl;});

    connect(ui->zoom,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),[&](double val){std::cout << ManProb->zoom;});

    connect(ui->add_point,&QPushButton::clicked,ManProb,[&]{
        ManProb->addPoint(point_i(spin_x,spin_y));
        ManProb->update();
    });
}

ManPowerGui::~ManPowerGui()
{
    delete ui;
}

