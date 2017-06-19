#include "qrcodereader.h"
#include "ui_qrcodereader.h"

QRcodereader::QRcodereader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QRcodereader)
{
    ui->setupUi(this);
}

QRcodereader::~QRcodereader()
{
    delete ui;
}
