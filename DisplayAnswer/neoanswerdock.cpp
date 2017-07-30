#include "neoanswerdock.h"
#include "ui_neoanswerdock.h"

NeoAnswerDock::NeoAnswerDock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NeoAnswerDock)
{
    ui->setupUi(this);
    // Background image
    QImage image("../../procon2017-comp/background.png");
    QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsPixmapItem* pic = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->addItem(pic);
    ui->Picture->setScene(scene);
}

NeoAnswerDock::~NeoAnswerDock()
{
    delete ui;
}

void NeoAnswerDock::addAnswer(const procon::Field &field)
{
    fields.push_back(field);
    AnswerBoard* answer_board = new AnswerBoard(fields.size()-1, true);
    answer_board->setField(field);
    answer_board->setFixedSize(300,300);
    connect(answer_board,&AnswerBoard::clicked_with_id,this,&NeoAnswerDock::clickedAnswer);
}

void NeoAnswerDock::clickedAnswer(int id)
{
    emit selectField(fields.at(id));
}
