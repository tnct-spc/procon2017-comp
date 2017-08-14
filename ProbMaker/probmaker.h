#ifndef PROBMAKER_H
#define PROBMAKER_H

#include <QWidget>
#include <string>
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

    void createPiece(polygon_i& argument_frame);
    void setInnerFrame(polygon_i frame);
    void checkClossLine(polygon_i& poly , polygon_i& change_frame);
    void jointPiece();
    void splitPiece();
    bool congruenceCheck();
    void erasePoint();
    void createFrame();

    bool IsCongruence(polygon_i polygon1 , polygon_i polygon2);//中野先輩のやつ

    std::vector<std::pair<point_i,point_i>> lines;
    std::vector<polygon_i> print_polygons;
    int retRnd(int pnu);
    int makecoordinate_x(int x);
    int makecoordinate_y(int y);
    void frame_generate(polygon_i ex,int a,int b);
    polygon_i frame;
    polygon_i check_frame;
    polygon_i inner_frame;

    int interbal_judge(int interbal, int keeping_x, int keeping_y, int target_x, int target_y);
    int north_judge(int interval,int keeping_x,int keeping_y,int target_x,int target_y);
    int northeast_judge(int interbal,int keeping_x,int keeping_y,int target_x,int target_y);
    int east_judge(int interbal,int keeping_x,int keeping_y,int target_x,int target_y);
    int southeast_judge(int interbal,int keeping_x,int keeping_y,int target_x,int target_y);
    int south_judge(int interbal,int keeping_x,int keeping_y,int target_x,int target_y);
    int southwest_judge(int interbal,int keeping_x,int keeping_y,int target_x,int target_y);
    int west_judge(int interbal,int keeping_x,int keeping_y,int target_x,int target_y);
    int northwest_judge(int interbal,int keeping_x,int keeping_y,int target_x,int target_y);
    int judge(int temporary_x,int temporary_y,int target_x,int target_y);
    int coordinate_x(int direction,int interbal,int keeping_x);
    int coordinate_y(int direction,int interbal,int keeping_y);
    int interbal(int keeping_x,int keeping_y,int target_x,int target_y);

    //void push_backer(int keeping_x,int keeping_y,int target_x,int target_y,polygon_i real_frame);

    int polygon_size = 4750;

private slots:
    void step();

signals:
    void nextLoop();

protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);
};

#endif // PROBMAKER_H
