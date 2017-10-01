#include "singlepolygondock.h"
#include "ui_singlepolygondock.h"

SinglePolygonDock::SinglePolygonDock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SinglePolygonDock)
{
    ui->setupUi(this);
}

SinglePolygonDock::~SinglePolygonDock()
{
    delete ui;
}
