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

    go_button = new QPushButton();
    go_button->setText("RUN BEAMSEARCH");

    ui->verticalLayout->addWidget(board,0);
    ui->verticalLayout->addWidget(go_button,1);
}

TryNextSearch::~TryNextSearch()
{
    delete ui;
}
