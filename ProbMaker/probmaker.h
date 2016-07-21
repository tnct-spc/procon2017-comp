#ifndef PROBMAKER_H
#define PROBMAKER_H

#include <random>
#include <vector>
#include <memory>
#include <algorithm>
#include <QMainWindow>

#include "displayanswer.h"


//prototype
class Dot;
class Line;

class Point
{
public:
    double x;
    double y;
    Point(double x_, double y_):x(x_),y(y_){}
};

class Dot
{
public:
    const double x;
    const double y;
    std::vector<std::shared_ptr<Line>> connectedLines;

    Dot(double x_,double y_):x(x_),y(y_){}
    bool operator==(const Dot &dot) const
    {
        return this->x == dot.x && this->y == dot.y;
    }
    bool removeConnectedLine(std::shared_ptr<Line> line)
    {
        auto begin = connectedLines.begin();
        auto end   = connectedLines.end();
        for(auto it = begin; it != end; it++){
            if(*it == line){
                connectedLines.erase(it);
                return 1;
            }
        }
        return 0;
    }
};

class Line
{
public:
    const std::shared_ptr<Dot> dot1;//start point
    const std::shared_ptr<Dot> dot2;//end point
    const double length = 0.0;//nolm
    const double can_connection_line_length = 0.0;
    const double angle = 0.0;
    const double a = 0.0, b = 0.0;//y=ax+b
    Line(const std::shared_ptr<Dot> &dot1_, const std::shared_ptr<Dot> &dot2_):dot1(dot1_),dot2(dot2_)
    {
        double lineX = dot2->x - dot1->x;
        double lineY = dot2->y - dot1->y;
        const_cast<double&>(length) = sqrt(lineX * lineX + lineY * lineY);
        const_cast<double&>(can_connection_line_length) = length-30 < 0 ? 0 : length-30;//5mm * 3pixcel * left-right
        const_cast<double&>(angle) = atan2(lineY,lineX);
        const_cast<double&>(a) = lineY/lineX;
        const std::shared_ptr<Dot> &dot_buf = dot2->x - dot1->x > 0 ? dot1:dot2;
        const_cast<double&>(b) = dot_buf->y - a * dot_buf->x;
    }
    //多角形生成時
    bool referenced_from_dot1 = false;
    bool referenced_from_dot2 = false;
};

namespace Ui {
class ProbMaker;
}

class ProbMaker : public QMainWindow
{
    Q_OBJECT

private:
    Ui::ProbMaker *ui;

    std::vector<std::shared_ptr<Dot>> dots;
    std::list<std::shared_ptr<Line>> lines;

    struct polygon{
        std::shared_ptr<Line> line;
        std::shared_ptr<Dot> s_dot;
        std::shared_ptr<Dot> e_dot;
    };

    std::vector<std::vector<struct polygon>> Polygons;

    std::shared_ptr<Line> latest_line;

    void addNewLine();
    void pushFlame();
    std::shared_ptr<Dot> pickRandomDotAtAll(std::shared_ptr<Line> &line_pos);
    std::shared_ptr<Dot> pickRandomDotOnRing(const std::vector<std::shared_ptr<Line>> lines,std::shared_ptr<Line> &line_pos);
    bool isCross(const std::shared_ptr<Line> &line1, const std::shared_ptr<Line> &line2);
    bool isValid(const std::shared_ptr<Line> &newL, double startL_angle, double endL_angle);
    void makePolygon();
    void eraseMinPolygon();

    DisplayAnswer* display;
public:


    explicit ProbMaker(QWidget *parent = 0);
    ~ProbMaker();
    void run();


    //test code
    int testCount = -2;
private slots:
    void clickStepButton();
    void clickTestButton();
protected:
    void paintEvent(QPaintEvent *);
};

#endif // PROBMAKER_H
