#include "before_neoanswerboard.h"
#include "ui_before-"

Before_NeoAnswerBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Before_NeoAnswerBoard)
{
    ui->setupUi(this);
}

Before_NeoAnswerBoard::~Before_NeoAnswerBoard()
{
    delete ui;
}
