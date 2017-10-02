#include "mygraphicsview.h"
#include "ui_mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyGraphicsView)
{
    ui->setupUi(this);
}

MyGraphicsView::~MyGraphicsView()
{
    delete ui;
}
