#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Dialog)
{
  ui->setupUi(this);
  connect(ui->pushButton, &QPushButton::clicked(),
                this, SLOT(Quit()));
  connect(ui->buttonBox, SIGNAL(accepted()),
                QRLibrary, SLOT(pause()));
  connect(ui->buttonBox, SIGNAL(accepted()),
                QRLibrary, SLOT(resume()));
  connect(ui->buttonBox, SIGNAL(rejected()),
                QRLibrary, SLOT(pause()));
}

Dialog::~Dialog()
{
  delete ui;
}

void Dialog::Quit()
{
  exit(0);
}
