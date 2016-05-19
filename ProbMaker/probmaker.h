#ifndef PROBMAKER_H
#define PROBMAKER_H

#include <QMainWindow>
#include <random>
#include <vector>
#include <memory>
#include <algorithm>

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
        const_cast<double&>(can_connection_line_length) = length - 30;
        if(can_connection_line_length < 0) const_cast<double&>(can_connection_line_length) = 0;
        const_cast<double&>(angle) = atan2(lineY,lineX);
        const_cast<double&>(a) = lineY/lineX;
        const std::shared_ptr<Dot> &dot = dot2->x - dot1->x > 0 ? dot1:dot2;
        const_cast<double&>(b) = dot->y - a * dot->x;
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

public:
    std::vector<std::shared_ptr<Dot>> dots;
    std::list<std::shared_ptr<Line>> lines;
    double can_connection_line_sum = 0.0;
    int line_piece_start = 0;
    int testCount = -1;
    std::shared_ptr<Line> newLine;
    typedef std::vector<Point> polygon_t;
    std::vector<polygon_t> Polygons;

    explicit ProbMaker(QWidget *parent = 0);
    ~ProbMaker();
    void run();
    void addNewLine();
    void pushFlame();
    std::shared_ptr<Dot> pickRandomDot(std::list<std::shared_ptr<Line>>::iterator &line_pos);
    void Check();
    bool isCross(const std::shared_ptr<Line> &line1, const std::shared_ptr<Line> &line2);
    bool isValid(const std::shared_ptr<Line> &newL, double startL_angle, double endL_angle);
    void makePolygon();
private slots:
    void clickStepButton();
    void clickTestButton();
protected:
    void paintEvent(QPaintEvent *);
};

#endif // PROBMAKER_H
