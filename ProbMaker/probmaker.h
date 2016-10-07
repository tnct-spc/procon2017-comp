#ifndef PROBMAKER_H
#define PROBMAKER_H

#include "field.h"
#include "expandedpolygon.h"
#include "polygon.h"
#include "polygonio.h"

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
    //多角形生成時に使用する
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

    struct polygon{
        std::shared_ptr<Line> line;
        std::shared_ptr<Dot> s_dot;
        std::shared_ptr<Dot> e_dot;
    };

    std::vector<std::shared_ptr<Dot>> dots;
    std::list<std::shared_ptr<Line>> lines;
    std::vector<std::vector<struct polygon>> Polygons;

    //新しいLineを引く
    void addNewLine();
    //枠線を引く
    void pushFrame();
    //すべての線からランダムでdotを作る
    std::shared_ptr<Dot> pickRandomDotAtAll(std::shared_ptr<Line> &line_pos);
    //ポリゴンの中からランダムでdotを作る
    std::shared_ptr<Dot> pickRandomDotOnRing(const std::vector<std::shared_ptr<Line>> lines,std::shared_ptr<Line> &line_pos);
    //線同士が重なっているか
    bool isCross(const std::shared_ptr<Line> &line1, const std::shared_ptr<Line> &line2);
    //角度や長さ、重なりなどを見て、引いても良い線なのか判別する
    bool isValidLine(const std::shared_ptr<Line> &newL, double startL_angle, double endL_angle);

    //ポリゴンを認識する
    void makePolygon();
    //ポリゴンからランダムな線を削除
    void eraseRandomLineOnPolygon(std::vector<struct polygon>& Polygon);
    //小さいポリゴンを削除
    void eraseMinPolygon();
    //ポリゴンの数が50になるまでポリゴンを削除
    void erasePolygonUnderFifty();
    //ポリゴンを拡張ポリゴンに変更
    //NOTE:PolygonSet->fieldに修正
    procon::Field PolygonToExPolygon();

public:
    explicit ProbMaker(QWidget *parent = 0);
    ~ProbMaker();
    void run();

protected:
    void paintEvent(QPaintEvent *);
};

#endif // PROBMAKER_H
