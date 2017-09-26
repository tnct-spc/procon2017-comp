#ifndef TRYNEXTSEARCH_H
#define TRYNEXTSEARCH_H

#include <QWidget>
#include "neoanswerboard.h"

namespace Ui {
class TryNextSearch;
}

class TryNextSearch : public QWidget
{
    Q_OBJECT

public:
    explicit TryNextSearch(QWidget *parent = 0);
    ~TryNextSearch();

private:
    Ui::TryNextSearch *ui;
    QPushButton *go_button;
    NeoAnswerBoard *board;
};

#endif // TRYNEXTSEARCH_H
