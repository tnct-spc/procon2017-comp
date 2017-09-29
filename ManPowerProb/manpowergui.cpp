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

    connect(ui->change_mode,&QPushButton::clicked,ManProb,&ManPowerProb::changeMode);
    connect(ui->delete_piece,&QPushButton::clicked,ManProb,&ManPowerProb::deletePiece);
    connect(ui->delete_frame,&QPushButton::clicked,ManProb,&ManPowerProb::deleteFrame);
    connect(ui->delete_point,&QPushButton::clicked,ManProb,&ManPowerProb::deletePoint);

}

ManPowerGui::~ManPowerGui()
{
    delete ui;
}

void ManPowerGui::deletePiece(){
    std::cout << "g" << std::endl;
}
