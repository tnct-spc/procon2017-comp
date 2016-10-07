#include "probmaker.h"
#include "ui_probmaker.h"

ProbMaker::ProbMaker(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProbMaker)
{
    ui->setupUi(this);
}

ProbMaker::~ProbMaker()
{
    delete ui;
}

void ProbMaker::run()
{
    //枠を作る
    pushFrame();

    //300回線を引く
    for(int i=0;i<300;i++) addNewLine();

    //ポリゴンを認識する
    makePolygon();

    //小さいポリゴンを削除する
    eraseMinPolygon();

    //ポリゴンの数が50個以下になるように減らす
    erasePolygonUnderFifty();

    //TestDisplayの再描画
    this->update();

    //認識したポリゴンを拡張ポリゴンに変換
    procon::Field field = PolygonToExPolygon();

    //ポリゴンをファイルに出力
    procon::PolygonIO::exportPolygon(field,"./../output.csv");

}

void ProbMaker::pushFrame()
{
    std::random_device rd;
    std::uniform_real_distribution<double> rand(0.0,1.0);

    //2cm以上空けて枠を作成
    double margin = 30 * 2;
    std::shared_ptr<Dot> dotA = std::make_shared<Dot>(margin+30*rand(rd),margin+30*rand(rd));
    std::shared_ptr<Dot> dotB = std::make_shared<Dot>(margin+30*rand(rd),900-(margin+30*rand(rd)));
    std::shared_ptr<Dot> dotC = std::make_shared<Dot>(900-(margin+30*rand(rd)),900-(margin+30*rand(rd)));
    std::shared_ptr<Dot> dotD = std::make_shared<Dot>(900-(60+margin*rand(rd)),(60+margin*rand(rd)));
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
    //2本の線を直線としたときの交点のX座標
    double cross_pointX = ((*line2).b - (*line1).b)/((*line1).a - (*line2).a);

    //X座標でくらべて、線どうしが交差しているかを判定
    bool is_cross = ((*line1).dot1->x < cross_pointX ? (*line1).dot1->x : (*line1).dot2->x) < cross_pointX &&
                    ((*line1).dot1->x < cross_pointX ? (*line1).dot2->x : (*line1).dot1->x) > cross_pointX &&
                    ((*line2).dot1->x < cross_pointX ? (*line2).dot1->x : (*line2).dot2->x) < cross_pointX &&
                    ((*line2).dot1->x < cross_pointX ? (*line2).dot2->x : (*line2).dot1->x) > cross_pointX;
    return is_cross;
}

bool ProbMaker::isValidLine(const std::shared_ptr<Line> &newL, double startL_angle, double endL_angle)
{
    //5mm以上の長さか
    if((*newL).length < 15.0) return false;

    //他の線と交差していないか
    for(auto it = lines.begin(), end = lines.end(); it != end; it++){
        if(isCross((*it), newL)) return false;
    }

    //25度より鋭い角度になっていないか

    auto NormAngle = [&](double angle){
        while(angle < 0.0) angle += M_PI;
        while(angle > M_PI) angle -= M_PI;
        return angle;
    };

    double normalized_newL_angle = NormAngle((*newL).angle);
    double normalized_startL_angle = NormAngle(startL_angle);
    double normalized_endL_angle = NormAngle(endL_angle);

    double accept_angle = 2 * M_PI * (25.0/360.0);

    double relative_startL_angle = normalized_startL_angle - normalized_newL_angle;
    double relative_endL_angle = normalized_endL_angle - normalized_newL_angle;
    if(relative_startL_angle < 0.0) relative_startL_angle += M_PI;
    if(relative_endL_angle < 0.0) relative_endL_angle += M_PI;
    if((M_PI - accept_angle) < relative_startL_angle || relative_startL_angle < accept_angle) return false;
    if((M_PI - accept_angle) < relative_endL_angle || relative_endL_angle < accept_angle) return false;

    return true;
}

procon::Field ProbMaker::PolygonToExPolygon()
{
    polygon_t frame;
    polygon_t buff;
    std::vector<polygon_t> pieces;
    procon::ExpandedPolygon ex_frame;
    procon::ExpandedPolygon buff2;
    std::vector<procon::ExpandedPolygon> ex_pieces;
    procon::Field field;

    //frame
    for(int j = 0;j < (static_cast<int>(Polygons.at(0).size()));++j){
        frame.outer().push_back(point_t(Polygons.at(0).at(j).s_dot->x/30,Polygons.at(0).at(j).s_dot->y/30));
    }
    frame.outer().push_back(point_t(Polygons.at(0).at(0).s_dot->x/30,Polygons.at(0).at(0).s_dot->y/30));
    ex_frame.resetPolygonForce(frame);
    field.setElementaryFrame(ex_frame);

    //polygon
    for(int i = 1;i < (static_cast<int>(Polygons.size()));++i){
        pieces.push_back(buff);
        //cal center
        double sumX=0.0,sumY=0.0;
        int polygon_size = Polygons.at(i).size();
        for(int j = 0;j < polygon_size;++j){
            sumX += Polygons.at(i).at(j).s_dot->x/30;
            sumY += Polygons.at(i).at(j).s_dot->y/30;
        }
        double reference_point_x = sumX / polygon_size;
        double reference_point_y = sumY / polygon_size;
        //put polygon
        for(int j = 0;j < (static_cast<int>(Polygons.at(i).size()));++j){
            pieces.at(i-1).outer().push_back(point_t((Polygons.at(i).at(j).s_dot->x/30)-reference_point_x,(Polygons.at(i).at(j).s_dot->y/30)-reference_point_y));
        }
        pieces.at(i-1).outer().push_back(point_t((Polygons.at(i).at(0).s_dot->x/30)-reference_point_x,(Polygons.at(i).at(0).s_dot->y/30)-reference_point_y));
        ex_pieces.push_back(buff2);
        ex_pieces.at(i-1).resetPolygonForce(pieces.at(i-1));
    }
    field.setElementaryPieces(ex_pieces);

    return field;
}

void ProbMaker::addNewLine()
{
    std::shared_ptr<Dot> start_dot;
    std::shared_ptr<Dot> end_dot;
    std::shared_ptr<Line> start_line;
    std::shared_ptr<Line> end_line;
    std::shared_ptr<Line> new_line;

    //生成するLineの始点と終点が決まるまで繰り返す
    while(true){
        //始点の決定
        start_dot = pickRandomDotAtAll(start_line);

        //終点の決定
        for(int i=0;i<10000;i++){
            end_dot = pickRandomDotAtAll(end_line);
            //有効な線になるか検査
            std::shared_ptr<Line> buff_line = std::make_shared<Line>(start_dot, end_dot);
            double a1 = start_line->angle;
            double a2 = end_line->angle;
            if(isValidLine(buff_line , a1,a2)) goto finishSelectLine;
        }
    }
    finishSelectLine:

    //新しいLineを生成（もともとあったLineを分裂させてくっつける）

    new_line = std::make_shared<Line>(start_dot,end_dot);
    std::shared_ptr<Line> divided_start_line_1 = std::make_shared<Line>(start_line->dot1, start_dot);
    std::shared_ptr<Line> divided_start_line_2 = std::make_shared<Line>(start_dot, start_line->dot2);
    std::shared_ptr<Line> divided_end_line_1 = std::make_shared<Line>(end_line->dot1, end_dot);
    std::shared_ptr<Line> divided_end_line_2 = std::make_shared<Line>(end_dot, end_line->dot2);
    //dotsとlinesに新しいdotと新しいlineを登録
    dots.push_back(start_dot);
    dots.push_back(end_dot);
    lines.push_back(new_line);
    //新しいdotに新しいlineを登録
    start_dot->connectedLines.push_back(new_line);
    start_dot->connectedLines.push_back(divided_start_line_1);
    start_dot->connectedLines.push_back(divided_start_line_2);
    end_dot->connectedLines.push_back(new_line);
    end_dot->connectedLines.push_back(divided_end_line_1);
    end_dot->connectedLines.push_back(divided_end_line_2);
    //すでにあったdotに新しいlineを登録
    start_line->dot1->removeConnectedLine(start_line);
    start_line->dot1->connectedLines.push_back(divided_start_line_1);
    start_line->dot2->removeConnectedLine(start_line);
    start_line->dot2->connectedLines.push_back(divided_start_line_2);
    end_line->dot1->removeConnectedLine(end_line);
    end_line->dot1->connectedLines.push_back(divided_end_line_1);
    end_line->dot2->removeConnectedLine(end_line);
    end_line->dot2->connectedLines.push_back(divided_end_line_2);
    //分裂前のLineを削除して、分裂後のLineを登録
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

void ProbMaker::eraseMinPolygon()
{
    bool eraseFlag;
    do{
        eraseFlag = false;
        for(auto polygons_it = Polygons.begin(),end = Polygons.end(); polygons_it < end; ++polygons_it){
            std::vector<struct polygon>& Polygon = *polygons_it;

            //[内包判定]
            auto includeCheck = [&](std::shared_ptr<Dot> dot){
                int cross_cnt = 0;
                //点からrandomDotに線を生成(ランダムでないと角っこにぶつかってしまいご判定？)
                std::random_device rd;
                std::uniform_real_distribution<double> rand(0.0,1.0);
                std::shared_ptr<Dot> zero_dot = std::make_shared<Dot>(-5+rand(rd),-5+rand(rd));
                std::shared_ptr<Line> line = std::make_shared<Line>(zero_dot,dot);
                //線と多角形との交差判定をカウント
                for(int i=0;i<(int)Polygon.size();++i){
                    if(isCross(line, Polygon.at(i).line)) cross_cnt++;
                }
                //奇数なら内包-true
                return cross_cnt%2 == 1 ? true : false;
            };

            //座標の最大最小xyを取得
            double minX=900,minY=900,maxX=0,maxY=0;
            for(auto dot_it = Polygon.begin(), end = Polygon.end(); dot_it < end; ++dot_it){
                if((*dot_it).s_dot->x < minX) minX = (*dot_it).s_dot->x;
                if((*dot_it).s_dot->x > maxX) maxX = (*dot_it).s_dot->x;
                if((*dot_it).s_dot->y < minY) minY = (*dot_it).s_dot->y;
                if((*dot_it).s_dot->y > maxY) maxY = (*dot_it).s_dot->y;
            }

            //正方形の始点を1/6cmずつずらす
            for(double x = minX; (x+30) < maxX; x += 5){
                for(double y = minY; (y+30) < maxY; y += 5){
                    //正方形の点を生成
                    std::array<std::shared_ptr<Dot>, 4> square_dot;
                    square_dot[0] = std::make_shared<Dot>(x   ,y   );
                    square_dot[1] = std::make_shared<Dot>(x   ,y+30);
                    square_dot[2] = std::make_shared<Dot>(x+30,y+30);
                    square_dot[3] = std::make_shared<Dot>(x+30,y   );
                    //正方形の点と多角形の[内包判定]
                    if(
                    includeCheck(square_dot[0]) &&
                    includeCheck(square_dot[1]) &&
                    includeCheck(square_dot[2]) &&
                    includeCheck(square_dot[3])
                    ){
                        //正方形の線を生成
                        std::array<std::shared_ptr<Line>, 4> square_line;
                        square_line[0] = std::make_shared<Line>(square_dot[0],square_dot[1]);
                        square_line[1] = std::make_shared<Line>(square_dot[1],square_dot[2]);
                        square_line[2] = std::make_shared<Line>(square_dot[2],square_dot[3]);
                        square_line[3] = std::make_shared<Line>(square_dot[3],square_dot[0]);
                        //正方形の線と多角形の[交差判定]
                        for(int i=0;i<4;++i){
                            for(int j=0;j<(int)Polygon.size();++j){
                                if(isCross(square_line[i], Polygon[j].line)) goto NextSquare;
                            }
                        }
                        //true
                        goto NextPolygon;
                        //false
                        NextSquare:;
                    }
                }
            }

            //正方形を含むことができなかったので、線をひとつ消す
            eraseFlag = true;
            eraseRandomLineOnPolygon(Polygon);

            NextPolygon:;
        }
        //作りなおす
        makePolygon();
    }while(eraseFlag==true);
}

void ProbMaker::erasePolygonUnderFifty()
{
    //50個以下になるまで、線の削除→ポリゴンの作り直し、を繰り返す
    while(Polygons.size() >= 50){

        //ランダムなポリゴンを指定
        std::random_device rd;
        std::uniform_real_distribution<double> rand(0.0,1.0);
        std::vector<struct polygon>& Polygon = Polygons.at(Polygons.size()*rand(rd));

        //線を一つ削除する
        eraseRandomLineOnPolygon(Polygon);

        //ポリゴンの再生成
        makePolygon();
    }
}

void ProbMaker::eraseRandomLineOnPolygon(std::vector<struct polygon>& Polygon)
{
    auto eraseLine = [&](std::shared_ptr<Line> line){
        for(auto i=lines.begin(),end=lines.end();i!=end;++i){
            if(*i == line){
                lines.erase(i);
                break;
            }
        }
    };

    auto eraseDot = [&](std::shared_ptr<Dot> dot){
        for(auto i=dots.begin(),end=dots.end();i!=end;++i){
            if(*i == dot){
                dots.erase(i);
                break;
            }
        }
    };

    std::function<void(std::shared_ptr<Dot> dot)> eraseStick = [&](std::shared_ptr<Dot> dot){
        if(dot->connectedLines.size() == 1){
            std::shared_ptr<Line> connectedLine = dot->connectedLines.at(0);
            std::shared_ptr<Dot> connectedDot = connectedLine->dot1 == dot ? connectedLine->dot2 : connectedLine->dot1;
            //Erase
            eraseDot(dot);
            connectedDot->removeConnectedLine(connectedLine);
            eraseLine(connectedLine);
            //Next
            eraseStick(connectedDot);
        }
    };

    //ランダムでポリゴンの一箇所の線を削除
    int size = Polygon.size();
    std::random_device rd;
    std::uniform_real_distribution<double> rand(0.0,1.0);
    int seek = size * rand(rd);
    struct polygon& erasedLine = Polygon.at(seek);
    erasedLine.s_dot->removeConnectedLine(erasedLine.line);
    erasedLine.e_dot->removeConnectedLine(erasedLine.line);
    eraseLine(erasedLine.line);

    //点に繋がってる線が一つだけなら再帰的に削除
    eraseStick(erasedLine.s_dot);
    if(erasedLine.e_dot->connectedLines.size()==0){
        eraseDot(erasedLine.e_dot);
    }else{
        eraseStick(erasedLine.e_dot);
    }
}

void ProbMaker::makePolygon()
{
    Polygons.clear();

    //すべてのdotに対して、繋がっているLineを角度順にソートする
    int dots_size = dots.size();
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
            std::vector<struct polygon> polygon;
            std::shared_ptr<Dot> start_dot = dot;
            std::shared_ptr<Dot> mine_dot = dot;
            std::shared_ptr<Line> mine_line = line;
            do{
                if(mine_line->dot1 == mine_dot){
                    mine_line->referenced_from_dot1 = true;
                    polygon.push_back({mine_line,mine_dot,mine_line->dot2});
                    mine_dot = mine_line->dot2;
                }else{
                    mine_line->referenced_from_dot2 = true;
                    polygon.push_back({mine_line,mine_dot,mine_line->dot1});
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
        painter.drawLine(QPointF(line->dot1->x/1.3+marginX, line->dot1->y/1.3+marginY), QPointF(line->dot2->x/1.3+marginX, line->dot2->y/1.3+marginY));
    };

    //すべてのlineを描画
    for(auto it = lines.begin(), end = lines.end(); it != end; it++){
        drawLine(*it);
    }
}
