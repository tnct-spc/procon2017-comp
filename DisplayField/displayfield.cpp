#include "displayfield.h"
#include "ui_displayfield.h"

DisplayField::DisplayField(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DisplayField)
{
    ui->setupUi(this);
    connect(ui->run_button,SIGNAL(clicked()),this,SLOT(runAnswerBoard()));
    connect(ui->select_button,SIGNAL(clicked()),this,SLOT(selectFieldData()));
    board = std::make_shared<NeoAnswerBoard>();
    board->show();
}

DisplayField::~DisplayField()
{
    delete ui;
}
void DisplayField::runAnswerBoard(){
    board->setField(field);
    board->setSingleMode(true);
    board->update();
}

void DisplayField::selectFieldData(){
    QString q_data = QFileDialog::getOpenFileName(this,"select file");
    std::string c_data = q_data.toStdString();
    NeoPolygonIO neoio;
    field = neoio.importField(c_data);
}
