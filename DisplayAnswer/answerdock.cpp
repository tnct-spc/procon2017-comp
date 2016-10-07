#include "answerdock.h"
#include "ui_answerdock.h"

#include "utilities.h"
#include "answerboard.h"
#include "field.h"
#include <QScrollArea>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>

AnswerDock::AnswerDock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnswerDock)
{
    ui->setupUi(this);
    //QImage image("../../procon2016-comp/yurikamome.jpg");
    //QGraphicsScene* scene = new QGraphicsScene();
    //QGraphicsPixmapItem* pic = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    //scene->addItem(pic);
    //ui->Picture->setScene(scene);
}

AnswerDock::~AnswerDock()
{
    delete ui;
}

void AnswerDock::addAnswer(procon::Field const& field)
{
    fields.push_back(field);
    AnswerBoard* answer_board = new AnswerBoard();
    answer_board->id = fields.size()-1;
    answer_board->setField(field);
    answer_board->SINGLE_MODE = true;
    answer_board->setFixedSize(300,300);
    connect(answer_board,&AnswerBoard::clicked_with_id,this,&AnswerDock::clickedAnswer);

    this->ui->testd->addWidget(answer_board,fields.size()/4,fields.size()%4);
}

void AnswerDock::clickedAnswer(int id)
{
    emit selectField(fields.at(id));
}
