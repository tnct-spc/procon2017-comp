#include "thresholdgui.h"
#include "ui_thresholdgui.h"

THresholdGUI::THresholdGUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::THresholdGUI)
{
    ui->setupUi(this);
}

THresholdGUI::~THresholdGUI()
{
    delete ui;
}
