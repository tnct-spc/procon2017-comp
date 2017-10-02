#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QWidget>

namespace Ui {
class MyGraphicsView;
}

class MyGraphicsView : public QWidget
{
    Q_OBJECT

public:
    explicit MyGraphicsView(QWidget *parent = 0);
    ~MyGraphicsView();

private:
    Ui::MyGraphicsView *ui;
};

#endif // MYGRAPHICSVIEW_H
