#include "trynextsearch.h"
#include "ui_trynextsearch.h"

#include <QLayout>

TryNextSearch::TryNextSearch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TryNextSearch)
{
    ui->setupUi(this);

    board = new NeoAnswerBoard();
    board->setSingleMode(true);
    board->setSelectPieceMode(true);

    go_button = new QPushButton();
    go_button->setText("RUN BEAMSEARCH");

    connect(go_button,&QPushButton::clicked,this,&TryNextSearch::clickedGoButton);

    ui->verticalLayout->addWidget(board,0);
    ui->verticalLayout->addWidget(go_button,1);
}

TryNextSearch::~TryNextSearch()
{
    delete ui;
}

void TryNextSearch::setField(procon::NeoField field){
    this->field = field;
    board->setField(this->field);
}

void TryNextSearch::clickedGoButton()
{
    std::cout << "clicked go button" << std::endl;

    emit startBeamSearch(this->field);
}
