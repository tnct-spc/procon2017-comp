#include "answerdock.h"
#include "ui_answerdock.h"

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
    QImage image("../../procon2016-comp/yurikamome.jpg");
    QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsPixmapItem* pic = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->addItem(pic);
    ui->Picture->setScene(scene);
}

AnswerDock::~AnswerDock()
{
    delete ui;
}

void AnswerDock::addAnswer(procon::Field const& new_field)
{
    this->fields.push_back(new_field);
    AnswerBoard* answer_board = new AnswerBoard();
    answer_board->setField(new_field);
    answer_board->setFixedSize(300,300);

    static int view_count = 0;
    this->ui->testd->addWidget(answer_board,view_count/4,view_count%4);
    view_count++;
}
