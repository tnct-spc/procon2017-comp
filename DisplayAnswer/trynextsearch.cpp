#include "trynextsearch.h"
#include "ui_trynextsearch.h"

TryNextSearch::TryNextSearch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TryNextSearch)
{
    ui->setupUi(this);
}

TryNextSearch::~TryNextSearch()
{
    delete ui;
}
