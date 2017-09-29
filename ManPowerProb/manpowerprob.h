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

    std::shared_ptr<spdlog::logger> logger;
    Ui::ManPowerProb *ui;
    QPointF translateToQPoint(point_i point);
    point_i translateToPointi(QPointF point);
    bool checkCanPlacePiece(point_i point);
    bool checkCanPlaceFrame(point_i point);
    void addPoint(point_i point);
    void exportToCsv();
    void createPolygon();
    void changeMode();

    int top_bottom_margin;
    int left_right_margin;
    int grid_size;
    procon::NeoField field;
    std::vector<polygon_i> pieces;
    std::vector<polygon_i> frames;
    bool is_frame = true;
    polygon_i not_put_part;


    polygon_i last_polygon;
    point_i standard_pos;


protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // MANPOWERPROB_H