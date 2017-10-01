#ifndef SINGLEPOLYGONDOCK_H
#define SINGLEPOLYGONDOCK_H

#include <QWidget>
#include "neoexpandedpolygon.h"
#include "neopolygonviewer.h"
#include <qscrollarea.h>


namespace Ui {
class SinglePolygonDock;
}

class SinglePolygonDock : public QWidget
{
    Q_OBJECT

public:
    explicit SinglePolygonDock(QWidget *parent = 0);
    ~SinglePolygonDock();

    void addPolygon(const polygon_i poly);

private:
    Ui::SinglePolygonDock *ui;

    std::vector<polygon_i> polygons;
};

#endif // SINGLEPOLYGONDOCK_H
