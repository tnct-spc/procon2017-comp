#ifndef MANPOWERPROB_H
#define MANPOWERPROB_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QWidget>
#include <qpainter.h>
#include <QMouseEvent>
#include <boost/geometry.hpp>
#include "neofield.h"
#include "neoexpandedpolygon.h"
#include "neopolygonio.h"
#include "spdlog/spdlog.h"



namespace Ui {
class ManPowerProb;
}

class ManPowerProb : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManPowerProb(QWidget *parent = 0);
    ~ManPowerProb();

private:
    Ui::ManPowerProb *ui;
    QPointF getPosition(point_i point);
    int top_bottom_margin;
    int left_right_margin;
    int grid_size;
    procon::NeoField field;
    std::vector<polygon_i> pieces;
    std::vector<polygon_i> frames;


protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // MANPOWERPROB_H
