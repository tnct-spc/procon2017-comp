#include "manpowergui.h"
#include "manpowerprob.h"
#include "ui_manpowergui.h"


ManPowerGui::ManPowerGui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManPowerGui)
{
    ui->setupUi(this);
    ManPowerProb *ManProb = new ManPowerProb();
    ManProb->show();

    int x,y;

    connect(ui->change_mode,&QPushButton::clicked,ManProb,&ManPowerProb::changeMode);
    connect(ui->delete_piece,&QPushButton::clicked,ManProb,&ManPowerProb::deletePiece);
    connect(ui->delete_frame,&QPushButton::clicked,ManProb,&ManPowerProb::deleteFrame);
    connect(ui->delete_point,&QPushButton::clicked,ManProb,&ManPowerProb::deletePoint);

    connect(ui->create_polygon,&QPushButton::clicked,ManProb,&ManPowerProb::createPolygon);

    connect(ui->set_point_x,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[&](int val){x=val;});
    connect(ui->set_point_x,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[&](int val){y=val;});
}

ManPowerGui::~ManPowerGui()
{
    delete ui;
}

void ManPowerGui::deletePiece(){
    std::cout << "g" << std::endl;
}
