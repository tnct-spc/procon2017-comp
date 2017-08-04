#ifndef PROBMAKER_H
#define PROBMAKER_H

#include <QWidget>
#include "neoexpandedpolygon.h"

namespace Ui {
class ProbMaker;
}

class ProbMaker : public QWidget
{
    Q_OBJECT

public:
    explicit ProbMaker(QWidget *parent = 0);
    ~ProbMaker();
    void delaunay_triangulation();
    void GA();
    void angulated_graphic();
    std::vector<polygon_i> getPieces();
    polygon_i getFrame();

private:
    Ui::ProbMaker *ui;
    std::vector<std::pair<point_i,point_i>> lines;
    std::vector<polygon_i> print_polygons;
    int retRnd(int pnum);
    polygon_i frame;
    int makecoordinate_x(int x);
    int makecoordinate_y(int y);
    void frame_generate(polygon_i ex,int a,int b);

    int polygon_size = 5000;

private slots:
    void step();

signals:
    void nextLoop();

protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);
};

#endif // PROBMAKER_H
