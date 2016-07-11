#include "probmaker.h"
#include "ui_probmaker.h"

#include <math.h>
#include <iostream>
#include <random>
#include <vector>
#include <memory>
#include <algorithm>
#include <QPainter>

ProbMaker::ProbMaker(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProbMaker)
{
    ui->setupUi(this);
    connect(ui->stepButton, &QPushButton::clicked, this, &ProbMaker::clickStepButton);
    connect(ui->polygonButton, &QPushButton::clicked, this, &ProbMaker::clickTestButton);
}

ProbMaker::~ProbMaker()
{
    delete ui;
}

void ProbMaker::pushFlame()
{
    //This is normal square flame
    std::shared_ptr<Dot> dotA = std::make_shared<Dot>(0,0);
    std::shared_ptr<Dot> dotB = std::make_shared<Dot>(0,900);
    std::shared_ptr<Dot> dotC = std::make_shared<Dot>(900,900);
    std::shared_ptr<Dot> dotD = std::make_shared<Dot>(900,0);
    std::shared_ptr<Line> lineAB = std::make_shared<Line>(dotA, dotB);
    std::shared_ptr<Line> lineBC = std::make_shared<Line>(dotB, dotC);
    std::shared_ptr<Line> lineCD = std::make_shared<Line>(dotC, dotD);
    std::shared_ptr<Line> lineDA = std::make_shared<Line>(dotD, dotA);
    dotA->connectedLines.push_back(lineAB);
    dotA->connectedLines.push_back(lineDA);
    dotB->connectedLines.push_back(lineBC);
    dotB->connectedLines.push_back(lineAB);
    dotC->connectedLines.push_back(lineCD);
    dotC->connectedLines.push_back(lineBC);
    dotD->connectedLines.push_back(lineDA);
    dotD->connectedLines.push_back(lineCD);
    dots.push_back(dotA);
    dots.push_back(dotB);
    dots.push_back(dotC);
    dots.push_back(dotD);
    lines.push_back(lineAB);
    lines.push_back(lineBC);
    lines.push_back(lineCD);
    lines.push_back(lineDA);
}

std::shared_ptr<Dot> ProbMaker::pickRandomDotAtAll(std::shared_ptr<Line> &line_pos)
{
    std::random_device rd;
    std::uniform_real_distribution<double> rand(0.0,1.0);

    //Select random point of dot on lines
    double sum_can_connection_line_length = 0.0;
    for (auto line_itr = lines.begin(), end = lines.end(); line_itr != end; ++line_itr){
        sum_can_connection_line_length += (*line_itr)->can_connection_line_length;
    }
    double seek = sum_can_connection_line_length * rand(rd);
    auto lines_it = lines.begin();
    while(seek >= 0.0){
        seek -= (*lines_it)->can_connection_line_length;
        lines_it++;
    }
    lines_it--;
    line_pos = *lines_it;
    seek += (*lines_it)->can_connection_line_length + 15;
    double dotX = seek * cos((*lines_it)->angle) + (*lines_it)->dot1->x;
    double dotY = seek * sin((*lines_it)->angle) + (*lines_it)->dot1->y;
    return std::make_shared<Dot>(dotX, dotY);
}

std::shared_ptr<Dot> ProbMaker::pickRandomDotOnRing(const std::vector<std::shared_ptr<Line>> lines, std::shared_ptr<Line> &line_pos)
{
    std::random_device rd;
    std::uniform_real_distribution<double> rand(0.0,1.0);

    //Select random point of dot on lines
    double sum_can_connection_line_length = 0.0;
    for (auto line_itr = lines.begin(), end = lines.end(); line_itr != end; ++line_itr){
        sum_can_connection_line_length += (*line_itr)->can_connection_line_length;
    }
    double seek = sum_can_connection_line_length * rand(rd);
    auto lines_it = lines.begin();
    while(seek >= 0.0){
        seek -= (*lines_it)->can_connection_line_length;
        lines_it++;
    }
    lines_it--;
    line_pos = *lines_it;
    seek += (*lines_it)->can_connection_line_length + 15;
    double dotX = seek * cos((*lines_it)->angle) + (*lines_it)->dot1->x;
    double dotY = seek * sin((*lines_it)->angle) + (*lines_it)->dot1->y;
    return std::make_shared<Dot>(dotX, dotY);
}

bool ProbMaker::isCross(const std::shared_ptr<Line> &line1, const std::shared_ptr<Line> &line2)
{
    //Get cross point x
    double cross_pointX = std::abs(((*line2).b - (*line1).b)/((*line2).a - (*line1).a));

    //Include check
    bool is_cross = ((*line1).dot1->x < cross_pointX ? (*line1).dot1->x : (*line1).dot2->x) <= cross_pointX &&
            cross_pointX <= ((*line1).dot1->x < cross_pointX ? (*line1).dot2->x : (*line1).dot1->x) &&
            ((*line2).dot1->x < cross_pointX ? (*line2).dot1->x : (*line2).dot2->x) <= cross_pointX &&
            cross_pointX <= ((*line2).dot1->x < cross_pointX ? (*line2).dot2->x : (*line2).dot1->x);
    return is_cross;
}

bool ProbMaker::isValid(const std::shared_ptr<Line> &newL, double startL_angle, double endL_angle)
{
    //Length check
    if((*newL).length < 15.0) return false;

    //Cross check
    for(auto it = lines.begin(), end = lines.end(); it != end; it++){
        if(isCross((*it), newL)) return false;
    }

    auto NormAngle = [&](double angle){
        double result = angle;
        if(angle < 0.0) result += 2 * M_PI;
        if(angle > M_PI) result -= M_PI;
        return result;
    };

    double normalized_newL_angle = NormAngle((*newL).angle);
    double normalized_startL_angle = NormAngle(startL_angle);
    double normalized_endL_angle = NormAngle(endL_angle);

    //Angle check
    double accept_angle = 2 * M_PI * (10.0/360.0);

    double relative_startL_angle = normalized_startL_angle - normalized_newL_angle;
    double relative_endL_angle = normalized_endL_angle - normalized_newL_angle;
    if(relative_startL_angle < 0.0) relative_startL_angle += M_PI;
    if(relative_endL_angle < 0.0) relative_endL_angle += M_PI;
    if((M_PI - accept_angle) < relative_startL_angle || relative_startL_angle < accept_angle) return false;
    if((M_PI - accept_angle) < relative_endL_angle || relative_endL_angle < accept_angle) return false;

    return true;
}

void ProbMaker::run()
{
    //Push flame
    pushFlame();

    /*
    for(int i=0;i<40;i++) addNewLine();
    makePolygon();
    this->update();
    */
}

void ProbMaker::addNewLine()
{
    std::shared_ptr<Dot> start_dot;
    std::shared_ptr<Dot> end_dot;
    std::shared_ptr<Line> new_line;
    std::shared_ptr<Line> start_line;
    std::shared_ptr<Line> end_line;

    //始点と終点を決める
    while(true){
		//始点の決定
        start_dot = pickRandomDotAtAll(start_line);

        //TODO: gutyoku
        for(int i=0;i<10000;i++){
            end_dot = pickRandomDotAtAll(end_line);
            std::shared_ptr<Line> bl = std::make_shared<Line>(start_dot, end_dot);
            double a1 = start_line->angle;
            double a2 = end_line->angle;
            if(isValid(bl , a1,a2)) goto finishSelectLine;
        }

        /*
		//終点の決定
		//Reset Line Reference
        for (auto line_itr = lines.begin(), end = lines.end(); line_itr != end; ++line_itr){
			(*line_itr)->referenced_from_dot1 = false;
			(*line_itr)->referenced_from_dot2 = false;
		}
        auto findNextLine = [&](std::vector<std::shared_ptr<Line>> &lines, std::shared_ptr<Line> &line){
            int size = lines.size();
            int i;
            for(i=0;i<size;i++){
                if(lines.at(i) == line) break;
            }
            i++;
            if(i==size) i=0;
            return lines.at(i);
        };
		//Search
        auto Search = [&](std::shared_ptr<Dot> start_dot_){
            start_dot = start_dot_;
            for (auto line_itr = start_dot->connectedLines.begin(),end = start_dot->connectedLines.end(); line_itr != end; ++line_itr){
                //Search Ring started by start_dot
                if (((*line_itr)->dot1 == start_dot && ((*line_itr)->referenced_from_dot1)) ||
                    ((*line_itr)->dot2 == start_dot && ((*line_itr)->referenced_from_dot2))) continue;
                //mine
                std::shared_ptr<Dot> mine_dot = start_dot;
                std::shared_ptr<Line> mine_line = *line_itr;
                std::vector<std::shared_ptr<Line>> mine_lines;
                do{
                    mine_lines.push_back(mine_line);
                    if (mine_line->dot1 == mine_dot){
                        mine_line->referenced_from_dot1 = true;
                        mine_dot = mine_line->dot2;
                    }
                    else{
                        mine_line->referenced_from_dot2 = true;
                        mine_dot = mine_line->dot1;
                    }
                    mine_line = findNextLine(mine_dot->connectedLines, mine_line);
                } while (mine_dot != start_dot);
                //if can make line, make.
                end_dot = pickRandomDotOnRing(mine_lines, end_line);
                bool can_make_line = isValid(std::make_shared<Line>(start_dot,end_dot), start_line->angle, end_line->angle);
                if(can_make_line) return 1;
            }
            return 0;
        };
        if(Search(start_line->dot1)) break;
        if(Search(start_line->dot2)) break;
        */
    }
    finishSelectLine:
    new_line = std::make_shared<Line>(start_dot,end_dot);
    latest_line = new_line;

    //regist dot and line
    dots.push_back(start_dot);
    dots.push_back(end_dot);
    lines.push_back(new_line);
    std::shared_ptr<Line> divided_start_line_1 = std::make_shared<Line>(start_line->dot1, start_dot);
    std::shared_ptr<Line> divided_start_line_2 = std::make_shared<Line>(start_dot, start_line->dot2);
    std::shared_ptr<Line> divided_end_line_1 = std::make_shared<Line>(end_line->dot1, end_dot);
    std::shared_ptr<Line> divided_end_line_2 = std::make_shared<Line>(end_dot, end_line->dot2);

    //refresh dots
    //newlineDot
    start_dot->connectedLines.push_back(new_line);
    start_dot->connectedLines.push_back(divided_start_line_1);
    start_dot->connectedLines.push_back(divided_start_line_2);
    end_dot->connectedLines.push_back(new_line);
    end_dot->connectedLines.push_back(divided_end_line_1);
    end_dot->connectedLines.push_back(divided_end_line_2);
    //start line dots
    start_line->dot1->removeConnectedLine(start_line);
    start_line->dot1->connectedLines.push_back(divided_start_line_1);
    start_line->dot2->removeConnectedLine(start_line);
    start_line->dot2->connectedLines.push_back(divided_start_line_2);
    //end line dots
    end_line->dot1->removeConnectedLine(end_line);
    end_line->dot1->connectedLines.push_back(divided_end_line_1);
    end_line->dot2->removeConnectedLine(end_line);
    end_line->dot2->connectedLines.push_back(divided_end_line_2);

    //refresh lines
    std::list<std::shared_ptr<Line>>::iterator start_line_it;
    for(auto i=lines.begin(),end=lines.end();i!=end;++i){
        if(*i == start_line){
            start_line_it = i;
            break;
        }
    }
    std::list<std::shared_ptr<Line>>::iterator end_line_it;
    for(auto i=lines.begin(),end=lines.end();i!=end;++i){
        if(*i == end_line){
            end_line_it = i;
            break;
        }
    }
    lines.erase(start_line_it);
    lines.push_back(divided_start_line_1);
    lines.push_back(divided_start_line_2);
    lines.erase(end_line_it);
    lines.push_back(divided_end_line_1);
    lines.push_back(divided_end_line_2);
}

void ProbMaker::clickStepButton()
{
    addNewLine();
    this->update();
}

void ProbMaker::clickTestButton()
{
    if(testCount==-1) makePolygon();
    testCount++;
    this->update();
}

void ProbMaker::makePolygon()
{
    int dots_size = dots.size();
    /*
    for(int dots_cnt=0; dots_cnt < dots_size; dots_cnt++){
        std::shared_ptr<Dot> &dot = dots.at(dots_cnt);
        std::cout<<"dots "<<dots_cnt<<std::endl;
        int size = dot->connectedLines.size();
        for(int i=0;i<size;i++){
            std::cout<<"("<<dot->connectedLines.at(i)->dot1->x<<","<<dot->connectedLines.at(i)->dot1->y<<")";
            std::cout<<"("<<dot->connectedLines.at(i)->dot2->x<<","<<dot->connectedLines.at(i)->dot2->y<<")";
            std::cout<<dot->connectedLines.at(i)->angle<<std::endl;
        }
    }
    */

    //All connectedLines sort
    for(int dots_cnt=0; dots_cnt < dots_size; dots_cnt++){
        std::shared_ptr<Dot> &dot = dots.at(dots_cnt);
        std::sort(dot->connectedLines.begin(), dot->connectedLines.end(),
                  [&](const std::shared_ptr<Line> &lhs, const std::shared_ptr<Line> &rhs){
            double a1 = (lhs->dot1 == dot)? lhs->angle : lhs->angle + M_PI;
            if(a1 > M_PI) a1 -= 2 * M_PI;
            double a2 = (rhs->dot1 == dot)? rhs->angle : rhs->angle + M_PI;
            if(a2 > M_PI) a2 -= 2 * M_PI;
            return  a1 > a2;
        });
    }

	//Reset Line Reference
    for (auto line_itr = lines.begin(), end = lines.end(); line_itr != end; ++line_itr){
		(*line_itr)->referenced_from_dot1 = false;
		(*line_itr)->referenced_from_dot2 = false;
	}

    //Search
    for(int dots_cnt=0; dots_cnt < dots_size; dots_cnt++){
        std::shared_ptr<Dot> &dot = dots.at(dots_cnt);
        int lines_size = dot->connectedLines.size();

        auto findNextLine = [&](std::vector<std::shared_ptr<Line>> &lines, std::shared_ptr<Line> &line){
            int size = lines.size();
            int i;
            for(i=0;i<size;i++){
                if(lines.at(i) == line) break;
            }
            i++;
            if(i==size) i=0;
            return lines.at(i);
        };

        for(int lines_cnt=0; lines_cnt < lines_size; lines_cnt++){
            std::shared_ptr<Line> &line = dot->connectedLines.at(lines_cnt);

            if((line->dot1 == dot && (line->referenced_from_dot1)) ||
               (line->dot2 == dot && (line->referenced_from_dot2))) continue;
            //mine
            polygon_t polygon;
            std::shared_ptr<Dot> start_dot = dot;
            std::shared_ptr<Dot> mine_dot = dot;
            std::shared_ptr<Line> mine_line = line;
            do{
                polygon.push_back(Point(mine_dot->x,mine_dot->y));
                if(mine_line->dot1 == mine_dot){
                    mine_line->referenced_from_dot1 = true;
                    mine_dot = mine_line->dot2;
                }else{
                    mine_line->referenced_from_dot2 = true;
                    mine_dot = mine_line->dot1;
                }
                mine_line = findNextLine(mine_dot->connectedLines, mine_line);
            }while(mine_dot != start_dot);
            Polygons.push_back(polygon);
        }
    }
}

void ProbMaker::paintEvent(QPaintEvent *)
{
    const int marginX = 60;
    const int marginY = 65;
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 1));

    auto drawLine = [&](std::shared_ptr<Line> line){
        painter.drawLine(QPointF(line->dot1->x+marginX, line->dot1->y+marginY), QPointF(line->dot2->x+marginX, line->dot2->y+marginY));
    };
    auto drawDot = [&](std::shared_ptr<Dot> dot){
        painter.drawPoint(dot->x+marginX,dot->y+marginY);
    };

    //Draw dot and line
    for(auto it = lines.begin(), end = lines.end(); it != end; it++){
        drawLine(*it);
    }
    if(lines.size()-1>=5){
        painter.setPen(QPen(Qt::red, 1));
        drawLine(latest_line);
        painter.setPen(QPen(Qt::red, 5));
        drawDot(latest_line->dot1);
    }

    //激やばテストコード
    if(testCount != -1){
        painter.setPen(QPen(Qt::red, 15));
        int num = Polygons.at(testCount).size();
        for(int i=0;i<num;i++){
            drawLine(std::make_shared<Line>(std::make_shared<Dot>(Polygons.at(testCount).at(i).x, Polygons.at(testCount).at(i).y),
                                            std::make_shared<Dot>(Polygons.at(testCount).at(i-1<0?num-1:i-1).x, Polygons.at(testCount).at(i-1<0?num-1:i-1).y)));
        }
    }
}
