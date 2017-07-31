#include "neoanswerboard.h"
#include "ui_neoanswerboard.h"

#include <QPainter>
#include <QPaintEvent>

NeoAnswerBoard::NeoAnswerBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NeoAnswerBoard)
{
    ui->setupUi(this);
}

NeoAnswerBoard::~NeoAnswerBoard()
{
    delete ui;
}

void NeoAnswerBoard::paintEvent(QPaintEvent *event){

}
