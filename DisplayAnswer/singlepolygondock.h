#ifndef SINGLEPOLYGONDOCK_H
#define SINGLEPOLYGONDOCK_H

#include <QWidget>

namespace Ui {
class SinglePolygonDock;
}

class SinglePolygonDock : public QWidget
{
    Q_OBJECT

public:
    explicit SinglePolygonDock(QWidget *parent = 0);
    ~SinglePolygonDock();

private:
    Ui::SinglePolygonDock *ui;
};

#endif // SINGLEPOLYGONDOCK_H
