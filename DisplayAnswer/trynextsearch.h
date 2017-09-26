#ifndef TRYNEXTSEARCH_H
#define TRYNEXTSEARCH_H

#include <QWidget>

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
};

#endif // TRYNEXTSEARCH_H
