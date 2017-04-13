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

    // Background image
    QImage image("../../procon2017-comp/background.png");
    QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsPixmapItem* pic = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->addItem(pic);
    ui->Picture->setScene(scene);
}

AnswerDock::~AnswerDock()
{
    delete ui;
}

void AnswerDock::addAnswer(procon::Field const& field)
{
    fields.push_back(field);
    AnswerBoard* answer_board = new AnswerBoard(fields.size()-1, true);
    answer_board->setField(field);
    answer_board->setFixedSize(300,300);
    connect(answer_board,&AnswerBoard::clicked_with_id,this,&AnswerDock::clickedAnswer);

    this->ui->board_container->addWidget(answer_board,fields.size()/4,fields.size()%4);
}

void AnswerDock::clickedAnswer(int id)
{
    emit selectField(fields.at(id));
}
