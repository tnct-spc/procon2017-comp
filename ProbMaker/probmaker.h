#ifndef PROBMAKER_H
#define PROBMAKER_H

#include <QWidget>
#include "neoexpandedpolygon.h"
#include "neofield.h"

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
    std::vector<polygon_i> getPieces();
    polygon_i getFrame();
    void makeHint(procon::NeoField field);
    std::string  shapeHints; //形状情報
    std::string disposition1,disposition2,disposition3,disposition4; //配置情報
    std::vector<std::string> disposition;
    int retRnd(int num);

private:
    Ui::ProbMaker *ui;
    std::vector<std::pair<point_i,point_i>> lines;
    std::vector<polygon_i> print_polygons;
    polygon_i frame;

private slots:
    void step();

signals:
    void nextLoop();

protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);
};

#endif // PROBMAKER_H
