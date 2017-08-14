#include "probmaker.h"
#include "ui_probmaker.h"
#include "neopolygonviewer.h"

#include <boost/geometry/geometry.hpp>
#include <boost/math/tools/fraction.hpp>
#include <boost/math/common_factor_rt.hpp>
#include <boost/polygon/voronoi.hpp>
#include <boost/assign/list_of.hpp>

#include <opencv2/imgproc/imgproc.hpp>

#include <numeric>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

#include <QPainter>
#include <QPushButton>
#include <QKeyEvent>
#include <QEventLoop>



ProbMaker::ProbMaker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProbMaker)
{
    ui->setupUi(this);
//    connect(this->ui->next_phase,&QPushButton::clicked,this,&ProbMaker::run);
//    this->run();
    //本番の101*65
//    polygon_i base_polygon;
//    base_polygon.outer().push_back(point_i(0,0));
//    base_polygon.outer().push_back(point_i(0,65));
//    base_polygon.outer().push_back(point_i(101,65));
//    base_polygon.outer().push_back(point_i(101,0));
//    base_polygon.outer().push_back(point_i(0,0));
//    this->print_polygons.push_back(base_polygon);

//    polygon_i poly_a;
//    bg::exterior_ring(poly_a) = boost::assign::list_of<point_i>
//        (0, 0)
//        (2, 0)
//        (2, 2)
//        (0, 2)
//        (0, 0)
//        ;
//    polygon_i poly_b;
//    bg::exterior_ring(poly_b) = boost::assign::list_of<point_i>
//        (2, 0)
//        (4, 0)
//        (4, 2)
//        (20, 20)
//        (2, 0)
//        ;

//    std::vector<polygon_i> polyyygon;
//    boost::geometry::union_(poly_a,poly_b,polyyygon);
//    std::cout << boost::geometry::intersects(poly_a,poly_b) << std::endl;

//    NeoPolygonViewer::getInstance().displayPolygon(poly_a,"aaaa",true);
//    NeoPolygonViewer::getInstance().displayPolygon(poly_b,"bbbb",true);
////    NeoPolygonViewer::getInstance().displayPolygon(polyyygon[0]);
//    for(auto pol : polyyygon){
//        NeoPolygonViewer::getInstance().displayPolygon(pol,"hgoehgoe",true);
//    }


//        //ひとつめの引数
//    polygon_i poly_1;
//    boost::geometry::exterior_ring(poly_1) = boost::assign::list_of<point_i>
//            (0,0)
//            (5,1)
//            (4,5)
//            (0,0)
//            ;
//    boost::geometry::reverse(poly_1);
//    //ふたつめの引数
//    polygon_i poly_2;
//    boost::geometry::exterior_ring(poly_2) = boost::assign::list_of<point_i>
//            (4,5)
//            (10,6)
//            (5,1)
//            (4,5)
//            ;

//    std::cout << boost::geometry::area(poly_1) << std::endl;
//    std::cout << boost::geometry::area(poly_2) << std::endl;

//    //くっつける部分
//    std::vector<polygon_i> vector;
//    boost::geometry::union_(poly_1,poly_2,vector);
//    polygon_i poly3;
//    poly3=vector.at(0);
//    //表示部分
//    std::cout<<vector.size()<<std::endl;
//    std::cout<<boost::geometry::dsv(poly3)<<std::endl;
//    NeoPolygonViewer::getInstance().displayPolygon(poly3,"connected",true);
//    NeoPolygonViewer::getInstance().displayPolygon(poly_1,"polygon1",true);
//    NeoPolygonViewer::getInstance().displayPolygon(poly_2,"polygon2",true);

    //ドロネーの三角形分割
//    delaunay_triangulation();
//    GA();
    angulated_graphic();
}

ProbMaker::~ProbMaker()
{
    delete ui;
}


int ProbMaker::retRnd(int num){
    std::random_device rand;
    std::mt19937 mt(rand());
    std::uniform_int_distribution<int> rnd(0,num-1);//randomを返すだけ
    std::cout << rnd(mt) << std::endl;
    return rnd(mt);
}
/*

int ProbMaker::judge(int temporary_x, int temporary_y, int target_x, int target_y){           //マス内に入っているか判定する関数
    if((temporary_x > target_x) && (temporary_x < target_x+25) && (temporary_y > target_y)&&(temporary_y < target_y+16)){
        return 1000;
    }else{
        return 0;
    }
}

int ProbMaker::north_judge(int interbal,int keeping_x,int keeping_y,int target_x,int target_y){  //点からみて北側に動けるか判定
    int temporary_x = keeping_x;
    int temporary_y = keeping_y - interbal;
    int a = judge(temporary_x,temporary_y,target_x,target_y);
    if(a == 1000){
        return 1000;
    }else{
        return 0;
    }
}
int ProbMaker::northeast_judge(int interbal,int keeping_x, int keeping_y, int target_x, int target_y){//点から見て北東側に動けるか判定
    int temporary_x = keeping_x + interbal;
    int temporary_y = keeping_y - interbal;
    int a = judge(temporary_x,temporary_y,target_x,target_y);
    if(a == 1000){
        return 1000;
    }else{
        return 0;
    }
}
int ProbMaker::east_judge(int interbal,int keeping_x,int keeping_y, int target_x, int target_y){//点から見て東側に動けるか
    int temporary_x = keeping_x + interbal;
    int temporary_y = keeping_y;
    int a = judge(temporary_x,temporary_y,target_x,target_y);
    if(a == 1000){
        return 1000;
    }else{
        return 0;
    }
}
int ProbMaker::southeast_judge(int interbal,int keeping_x,int keeping_y, int target_x, int target_y){//点から見て南東側に動けるか
    int temporary_x = keeping_x + interbal;
    int temporary_y = keeping_y + interbal;
    int a = judge(temporary_x,temporary_y,target_x,target_y);
    if(a == 1000){
        return 1000;
    }else{
        return 0;
    }
}
int ProbMaker::south_judge(int interbal,int keeping_x,int keeping_y, int target_x, int target_y){//点から見て南側に動けるか
    int temporary_x = keeping_x;
    int temporary_y = keeping_y + interbal;
    int a = judge(temporary_x,temporary_y,target_x,target_y);
    if(a == 1000){
        return 1000;
    }else{
        return 0;
    }
}
int ProbMaker::southwest_judge(int interbal,int keeping_x,int keeping_y, int target_x, int target_y){//点から見て南西側に動けるか
    int temporary_x = keeping_x - interbal;
    int temporary_y = keeping_y + interbal;
    int a = judge(temporary_x,temporary_y,target_x,target_y);
    if(a == 1000){
        return 1000;
    }else{
        return 0;
    }
}
int ProbMaker::west_judge(int interbal,int keeping_x,int keeping_y, int target_x, int target_y){//点から見て西側に動けるか
    int temporary_x = keeping_x - interbal;
    int temporary_y = keeping_y;
    int a = judge(temporary_x,temporary_y,target_x,target_y);
    if(a == 1000){
        return 1000;
    }else{
        return 0;
    }
}
int ProbMaker::northwest_judge(int interbal,int keeping_x,int keeping_y, int target_x, int target_y){//点から見て北西側に動けるか
    int temporary_x = keeping_x - interbal;
    int temporary_y = keeping_y - interbal;
    int a = judge(temporary_x,temporary_y,target_x,target_y);
    if(a == 1000){
        return 1000;
    }else{
        return 0;
    }
}
int ProbMaker::interbal_judge(int interbal,int keeping_x,int keeping_y,int target_x,int target_y){
    std::vector<int> memory;
   memory.clear();
   int N = north_judge(interbal,keeping_x,keeping_y,target_x,target_y);
   if(N == 1000){
       memory.push_back(1);
   }
   int NE = northeast_judge(interbal,keeping_x,keeping_y,target_x,target_y);
   if(NE == 1000){
       memory.push_back(2);
   }
   int E = east_judge(interbal,keeping_x,keeping_y,target_x,target_y);
   if(E == 1000){
       memory.push_back(3);
   }
   int SE = southeast_judge(interbal,keeping_x,keeping_y,target_x,target_y);
   if(SE == 1000){
       memory.push_back(4);
   }
   int S = south_judge(interbal,keeping_x,keeping_y,target_x,target_y);
   if(S == 1000){
       memory.push_back(5);
   }
   int SW = southwest_judge(interbal,keeping_x,keeping_y,target_x,target_y);
   if(SW == 1000){
       memory.push_back(6);
   }
   int W = west_judge(interbal,keeping_x,keeping_y,target_x,target_y);
   if(W == 1000){
       memory.push_back(7);
   }
   int NW = northwest_judge(interbal,keeping_x,keeping_y,target_x,target_y);
   if(NW == 1000){
       memory.push_back(8);
        }
   //int memory_size = memory.size();
   if((N == 1000)||(NE==1000)||(E == 1000)||(SE == 1000)||(S==1000)||(SW == 1000)||(W == 1000)||(NW == 1000)){
       return memory[retRnd(sizeof(memory))];
   }else{
       return 0;
   }
}

/*
void ProbMaker::push_backer(int keeping_x,int keeping_y, int target_x, int target_y,polygon_i real_frame){
    int decision;
    int interval;
    do{
    interval = retRnd(20);
    decision = interbal_judge(interval,keeping_x,keeping_y,target_x,target_y);
    }while(decision == 0);
    int coordinate_x,coordinate_y;
    if(decision == 1){
        coordinate_x = keeping_x;
        coordinate_y = keeping_y - interval;
    }
    if(decision == 2){
        coordinate_x = keeping_x + interval;
        coordinate_y = keeping_y - interval;
    }
    if(decision == 3){
        coordinate_x = keeping_x + interval;
        coordinate_y = keeping_y;
    }
    if(decision == 4){
        coordinate_x = keeping_x + interval;
        coordinate_y = keeping_y + interval;
    }
    if(decision == 5){
        coordinate_x = keeping_x;
        coordinate_y = keeping_y + interval;
    }
    if(decision == 6){
        coordinate_x = keeping_x - interval;
        coordinate_y = keeping_y + interval;
    }
    if(decision == 7){
        coordinate_x = keeping_x - interval;
        coordinate_y = keeping_y;
    }
    if(decision == 8){
        coordinate_x = keeping_x - interval;
        coordinate_y = keeping_y - interval;
    }
    real_frame.outer().push_back(point_i(coordinate_x,coordinate_y));
    keeping_x = coordinate_x;
    keeping_y = coordinate_y;
}
*/
/*
int ProbMaker::coordinate_x(int direction,int interbal, int keeping_x){
    if(direction == 1){
        return keeping_x;
    }
    if(direction == 2){
        return keeping_x + interbal;
    }
    if(direction == 3){
        return keeping_x + interbal;
    }
    if(direction == 4){
        return keeping_x + interbal;
    }
    if(direction == 5){
        return keeping_x;
    }
    if(direction == 6){
        return keeping_x - interbal;
    }
    if(direction == 7){
        return keeping_x - interbal;
    }
    if(direction == 8){
        return keeping_x - interbal;
    }
}
int ProbMaker::coordinate_y(int direction, int interbal, int keeping_y){
    if(direction == 1){
        return keeping_y - interbal;
    }
    if(direction == 2){
        return keeping_y - interbal;
    }
    if(direction == 3){
        return keeping_y;
    }
    if(direction == 4){
        return keeping_y + interbal;
    }
    if(direction == 5){
        return keeping_y + interbal;
    }
    if(direction == 6){
        return keeping_y + interbal;
    }
    if(direction == 7){
        return keeping_y;
    }
    if(direction == 8){
        return keeping_y - interbal;
    }
}
*/

void ProbMaker::angulated_graphic(){
    polygon_i real_frame;//本物の枠

    int firstcoordinate_x,firstcoordinate_y;

    std::vector<int> vertex1;
    do{
        bg::clear(real_frame);
    for(int a = 0;a < 4;a++){
         vertex1.push_back(retRnd(4));
    }
    int inter;
    std::set<int> anaraiz(vertex1.begin(),vertex1.end());
    std::cout<<"set is done"<<std::endl;
    std::vector<int> vertex(anaraiz.begin(),anaraiz.end());
    std::cout <<"vector is done" << std::endl;
    int count0 = 0,count1 = 0,count2 = 0,count3 = 0;
    std::cout << "int is done"<<std::endl;
    int num;
    for(int count = 0;count != vertex.size();count++){
        num = vertex[count];
        if(num == 0){
            count0 = 1;
        }
        if(num == 1){
            count1 = 1;
        }
        if(num == 2){
            count2 = 1;
        }
        if(num == 3){
            count3 = 1;
        }
    }
    std::cout <<"count is done"<<std::endl;
    inter = retRnd(32);
    if(count0 == 1){
        real_frame.outer().push_back(point_i(inter,0));
        real_frame.outer().push_back(point_i(0,inter));
        firstcoordinate_x = inter,firstcoordinate_y = 0;
    }else{
        real_frame.outer().push_back(point_i(0,0));
        firstcoordinate_x = 0,firstcoordinate_y = 0;
    }
    inter = retRnd(32);
    if(count1 == 1){
        real_frame.outer().push_back(point_i(0,64-inter));
        real_frame.outer().push_back(point_i(inter,64));
    }else{
        real_frame.outer().push_back(point_i(0,64));
    }
    inter = retRnd(32);
    if(count2 == 1){
        real_frame.outer().push_back(point_i(100-inter,64));
        real_frame.outer().push_back(point_i(100,64-inter));
    }else{
        real_frame.outer().push_back(point_i(100,64));
    }
    inter = retRnd(32);
    if(count3 == 1){
        real_frame.outer().push_back(point_i(100,inter));
        real_frame.outer().push_back(point_i(100-inter,0));
    }else{
        real_frame.outer().push_back(point_i(100,0));
    }
    real_frame.outer().push_back(point_i(firstcoordinate_x,firstcoordinate_y));
}while(bg::area(real_frame) > polygon_size);
/*
   int interbalnumber = 50;
   int firstcoordinate_x = retRnd(25);
   int firstcoordinate_y = retRnd(16);
   int keeping_x = firstcoordinate_x;
   int keeping_y = firstcoordinate_y;

   real_frame.outer().push_back(point_i(firstcoordinate_x,firstcoordinate_y));


   std::cout << "一つ目の頂点#############################################################################################3"
                "########################################################################################################"
                "#######################################################################################################"
                "######################################################################################################"
                "##########################################################################################################"
                "#########################################################################################################"
                "############################################################################################333333333333" <<std::endl;
   int box_interbal_2,box_direction_2;
   do{
   box_interbal_2 = retRnd(interbalnumber);
   box_direction_2 = interbal_judge(box_interbal_2,keeping_x,keeping_y,0,16);
   }while(box_direction_2 == 0);
   int coordinate_x2 = coordinate_x(box_direction_2,box_interbal_2,keeping_x),coordinate_y2 = coordinate_y(box_direction_2,box_interbal_2,keeping_y);
   real_frame.outer().push_back(point_i(coordinate_x2,coordinate_y2));
   keeping_x = coordinate_x2;
   keeping_y = coordinate_y2;
   std::cout <<"二つ目の頂点#################################################################################################"
               "3333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333"
               "3333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333"
               "33333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333"
               "3333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333"
               "33333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333" <<std::endl;
   //################################################################################################################################################################3
   int box_interbal_3,box_direction_3;
   do{
   box_interbal_3 = retRnd(interbalnumber);
   box_direction_3 = interbal_judge(box_interbal_3,keeping_x,keeping_y,0,32);
   }while(box_direction_3 == 0);
   int coordinate_x3 = coordinate_x(box_direction_3,box_interbal_3,keeping_x),coordinate_y3 = coordinate_y(box_direction_3,box_interbal_3,keeping_y);
   real_frame.outer().push_back(point_i(coordinate_x3,coordinate_y3));
   keeping_x = coordinate_x3;
   keeping_y = coordinate_y3;
   std::cout <<"三つ目の頂点###############################################################################################"<<std::endl;
   //######################################################################################################################################################################
   int box_interbal_4,box_direction_4;
   do{
       box_interbal_4 = retRnd(interbalnumber);
       box_direction_4 = interbal_judge(box_interbal_4,keeping_x,keeping_y,0,48);
   }while(box_direction_4 == 0);
   int coordinate_x4 = coordinate_x(box_direction_4,box_interbal_4,keeping_x),coordinate_y4 = coordinate_y(box_direction_4,box_interbal_4,keeping_y);
   real_frame.outer().push_back(point_i(coordinate_x4,coordinate_y4));
   keeping_x = coordinate_x4;
   keeping_y = coordinate_y4;
   std::cout <<"四つ目の頂点#################################################################################################" <<std::endl;
   //################################3333333333333333333333333333333#############################################################################
   int box_direction_5,box_interbal_5;
   do{
       box_interbal_5 = retRnd(interbalnumber);
       box_direction_5 = interbal_judge(box_interbal_5,keeping_x,keeping_y,25,48);
   }while(box_direction_5 == 0);
   int coordinate_x5 = coordinate_x(box_direction_5,box_interbal_5,keeping_x), coordinate_y5 = coordinate_y(box_direction_5,box_interbal_5,keeping_y);
   real_frame.outer().push_back(point_i(coordinate_x5,coordinate_y5));
   keeping_x = coordinate_x5;
   keeping_y = coordinate_y5;
   std::cout <<"五つ目の頂点##################################################################################################" <<std::endl;
   //#######################################################################################################################################################
   int box_direction_6,box_interbal_6;
   do{
       box_interbal_6 = retRnd(interbalnumber);
       box_direction_6 = interbal_judge(box_interbal_6,keeping_x,keeping_y,50,48);
   }while(box_direction_6 == 0);
   int coordinate_x6 = coordinate_x(box_direction_6,box_interbal_6,keeping_x);
   int coordinate_y6 = coordinate_y(box_direction_6,box_interbal_6,keeping_y);
   real_frame.outer().push_back(point_i(coordinate_x6,coordinate_y6));
   keeping_x = coordinate_x6,keeping_y = coordinate_y6;
   std::cout <<"六つ目の頂点###################################################################################################" <<std::endl;
   //#######################################################################################################################################
   int box_direction_7,box_interbal_7;
   do{
       box_interbal_7 = retRnd(interbalnumber);
       box_direction_7 = interbal_judge(box_interbal_7,keeping_x,keeping_y,75,48);
   }while(box_direction_7 == 0);
   int coordinate_x7 = coordinate_x(box_direction_7,box_interbal_7,keeping_x),coordinate_y7 = coordinate_y(box_direction_7,box_interbal_7,keeping_y);
   real_frame.outer().push_back(point_i(coordinate_x7,coordinate_y7));
   keeping_x = coordinate_x7,keeping_y = coordinate_y7;
   std::cout <<"七つ目の頂点###################################################################################################" <<std::endl;
   int box_direction_8,box_interbal_8;
   do{
       box_interbal_8 = retRnd(interbalnumber);
       box_direction_8 = interbal_judge(box_interbal_8,keeping_x,keeping_y,75,32);
   }while(box_direction_8 == 0);
   int coordinate_x8 = coordinate_x(box_direction_8,box_interbal_8,keeping_x),coordinate_y8 = coordinate_y(box_direction_8,box_interbal_8,keeping_y);
   real_frame.outer().push_back(point_i(coordinate_x8,coordinate_y8));
   keeping_x = coordinate_x8,keeping_y = coordinate_y8;
   std::cout <<"八つ目の頂点#####################################################################################################"<<std::endl;
   int box_interbal_9,box_direction_9;
   do{
       box_interbal_9 = retRnd(interbalnumber);
       box_direction_9 = interbal_judge(box_interbal_9,keeping_x,keeping_y,75,16);
   }while(box_direction_9 == 0);
   int coordinate_x9 = coordinate_x(box_direction_9,box_interbal_9,keeping_x),coordinate_y9 = coordinate_y(box_direction_9,box_interbal_9,keeping_y);
   real_frame.outer().push_back(point_i(coordinate_x9,coordinate_y9));
   keeping_x = coordinate_x9,keeping_y = coordinate_y9;
   std::cout <<"九つ目のの頂点####################################################################################################" <<std::endl;
   int box_direction_10,box_interbal_10;
   do{
       box_interbal_10 = retRnd(interbalnumber);
       box_direction_10 = interbal_judge(box_interbal_10,keeping_x,keeping_y,75,0);
   }while(box_direction_10 == 0);
   int coordinate_x10 = coordinate_x(box_direction_10,box_interbal_10,keeping_x),coordinate_y10 = coordinate_y(box_direction_10,box_interbal_10,keeping_y);
   real_frame.outer().push_back(point_i(coordinate_x10,coordinate_y10));
   keeping_x = coordinate_x10,keeping_y = coordinate_y10;
   std::cout <<"十個目の頂点#####################################################################################################" <<std::endl;
   int box_direction_11,box_interbal_11;
   do{
       box_interbal_11 = retRnd(interbalnumber);
       box_direction_11 = interbal_judge(box_interbal_11,keeping_x,keeping_y,50,0);
   }while(box_direction_11 == 0);
   int coordinate_x11 = coordinate_x(box_direction_11,box_interbal_11,keeping_x),coordinate_y11 = coordinate_y(box_direction_11,box_interbal_11,keeping_y);
   real_frame.outer().push_back(point_i(coordinate_x11,coordinate_y11));
   keeping_x = coordinate_x11,keeping_y = coordinate_y11;
   std::cout <<"十一個目の頂点####################################################################################################" <<std::endl;
   int box_direction_12,box_interbal_12;
   do{
       box_interbal_12 = retRnd(interbalnumber);
       box_direction_12 = interbal_judge(box_interbal_12,keeping_x,keeping_y,25,0);
   }while(box_direction_12 == 0);
   int coordinate_x12 = coordinate_x(box_direction_12,box_interbal_12,keeping_x),coordinate_y12 = coordinate_y(box_direction_12,box_interbal_12,keeping_y);
   real_frame.outer().push_back(point_i(coordinate_x12,coordinate_y12));
   keeping_x = coordinate_x12,keeping_y = coordinate_y12;
   std::cout <<"一二個目の頂点####################################################################################################" <<std::endl;
*/
   //real_frame.outer().push_back(point_i(firstcoordinate_x,firstcoordinate_y);
    std::cout << "このポリゴンの面積は" << bg::area(real_frame) << std::endl;

    polygon_i vertical;
    vertical.outer().push_back(point_i(50,0));
    vertical.outer().push_back(point_i(51,0));
    vertical.outer().push_back(point_i(51,64));
    vertical.outer().push_back(point_i(50,64));
    vertical.outer().push_back(point_i(50,0));

    polygon_i horizontal;
    horizontal.outer().push_back(point_i(0,32));
    horizontal.outer().push_back(point_i(0,33));
    horizontal.outer().push_back(point_i(100,33));
    horizontal.outer().push_back(point_i(100,32));
    horizontal.outer().push_back(point_i(0,32));



    print_polygons.push_back(real_frame);
    print_polygons.push_back(vertical);
    print_polygons.push_back(horizontal);
    //polygon_i sample_frame;//   テストで枠を生成
    //sample_frame.outer().push_back(point_i(12,0));
    //sample_frame.outer().push_back(point_i(80,0));
    //sample_frame.outer().push_back(point_i(101,20));
    //sample_frame.outer().push_back(point_i(101,65));
    //sample_frame.outer().push_back(point_i(20,65));
    //sample_frame.outer().push_back(point_i(20,52));
    //sample_frame.outer().push_back(point_i(0,12));
    //sample_frame.outer().push_back(point_i(12,0));
    frame = real_frame;
    //print_polygons.push_back(sample_frame);



}
void ProbMaker::delaunay_triangulation()
{

    std::mt19937 mt;
    std::uniform_int_distribution<int> x_distribution(0,101);
    std::uniform_int_distribution<int> y_distribution(0,65);
    std::vector<cv::Point2f> points;
    for (int var = 0; var < 100; ++var) {
        int x = x_distribution(mt);
        int y = y_distribution(mt);

        points.push_back(cv::Point2f(x,y));
    }

    cv::Subdiv2D subdiv;
    subdiv.initDelaunay(cv::Rect(0,0,150,100));
    subdiv.insert(points);

    std::vector<cv::Vec6f> triangles;
    subdiv.getTriangleList(triangles);
    for(auto vec : triangles){
        bool flag = false;

        auto check = [&](auto a){
            if(-300 > a || a > 300) flag = true;
        };

        for (int a = 0; a < 6; ++a) {
            check(vec[a]);
        }

        if(!flag){
            polygon_i poly_buf;
            poly_buf.outer().push_back(point_i(vec[0],vec[1]));
            poly_buf.outer().push_back(point_i(vec[2],vec[3]));
            poly_buf.outer().push_back(point_i(vec[4],vec[5]));
            poly_buf.outer().push_back(point_i(vec[0],vec[1]));

            //面積が負だとboostのpolygon conceptに怒られてしまうのです
            if(0 > boost::geometry::area(poly_buf)){
                boost::geometry::reverse(poly_buf);
            }

            this->print_polygons.push_back(poly_buf);
        }
    }

    polygon_i base_polygon;
    base_polygon.outer().push_back(point_i(0,0));
    base_polygon.outer().push_back(point_i(0,65));
    base_polygon.outer().push_back(point_i(101,65));
    base_polygon.outer().push_back(point_i(101,0));
    base_polygon.outer().push_back(point_i(0,0));

    std::vector<polygon_i> polygons;
    std::copy(this->print_polygons.begin(),this->print_polygons.end(),std::back_inserter(polygons));

    std::vector<polygon_i> out;
    unsigned int a,b;
    for (a = 0; a < polygons.size(); ++a) {
        for (b = 1; b < polygons.size(); ++b) {
            boost::geometry::union_(polygons.at(a),polygons.at(b),out);

            if(out.size() == 1){


                goto EXIT;
            }
        }
    }
    EXIT:
    //結合した分を消す
    if(a < b){
        polygons.erase(polygons.begin() + b);
        polygons.erase(polygons.begin() + a);
    }else{
        polygons.erase(polygons.begin() + a);
        polygons.erase(polygons.begin() + b);
    }
    polygon_i frame = out[0];

    while(polygons.size() != 0){
        for (unsigned int index = 0; index < polygons.size(); ++index) {
            std::vector<polygon_i> output_buffer;
            boost::geometry::union_(frame,polygons.at(index),output_buffer);

            if(output_buffer.size() == 1){
                polygons.erase(polygons.begin() + index);
                frame = output_buffer[0];
            }
        }
    }
    this->frame = frame;

//    NeoPolygonViewer::getInstance().displayPolygon(frame,"frame",true);

//    std::copy(this->print_polygons.begin(),this->print_polygons.end(),)

//    while()

//    NeoSinglePolygonDisplay::createInstance(this->print_polygons[0],"hogehoge")->show();
//    NeoSinglePolygonDisplay::createInstance(this->print_polygons[1],"fugapiyo")->show();
//    for (unsigned int index = 1; index < this->print_polygons.size(); ++index) {
//        boost::geometry::difference(out[0],this->print_polygons[index],out);
//    }

//    std::cout << boost::geometry::dsv(out[0]) << std::endl;
//    std::cout << out.size() << std::endl;

//    std::cout << "delaunay trianglation completed" << std::endl;

//    for(auto polygon : this->print_polygons){
//	    std::cout << boost::geometry::dsv(polygon) << std::endl;
//    }

}

void ProbMaker::GA()
{
    std::vector<polygon_i> polygons;
    std::copy(this->print_polygons.begin(),this->print_polygons.end(),std::back_inserter(polygons));
    polygon_i frame = this->frame;

    std::random_device rnd;

    auto connect_near_by_polygon = [](std::vector<polygon_i> pieces){

        std::vector<polygon_i> back_up_polygon;
        std::copy(pieces.begin(),pieces.end(),std::back_inserter(back_up_polygon));

ONCE_MORE:


        std::random_device rnd;
        //真に遺憾ですが、乱数を投入してしまいます
        std::uniform_int_distribution<> randomm(0,pieces.size() - 1);
        int connect_polygon_index = randomm(rnd);

        polygon_i connect_polygon = pieces[connect_polygon_index];

        pieces.erase(pieces.begin() + connect_polygon_index);

        int counter = 0;
        std::vector<int> can_connect_polygon_number;
        for(auto piece : pieces){
            std::vector<polygon_i> out;
            boost::geometry::union_(connect_polygon,piece,out);

            if(out.size() == 1){
                std::cout << "find can connect polygon" << std::endl;
                can_connect_polygon_number.push_back(counter);
            }

            ++counter;
        }

        if(can_connect_polygon_number.size() == 0){
            pieces.clear();
            std::copy(back_up_polygon.begin(),back_up_polygon.end(),std::back_inserter(pieces));
            goto ONCE_MORE;
        }

        //connect可能な中から適当に選ぶ
        //あたりまえだけど、-1してstd::out_of_range回避
        std::uniform_int_distribution<> random(0,can_connect_polygon_number.size() - 1);
        int connecting_polygon_index = random(rnd);



        counter = 0;
        std::vector<std::pair<double,int>> size;
        for(auto i : can_connect_polygon_number){
            std::pair<double,int> buf;
            buf.first = boost::geometry::area(pieces[i]);
            buf.second = counter;
            ++counter;
            size.push_back(buf);
        }

        std::sort(size.begin(),size.end(),[](auto a,auto b){
            return a.first < b.first;
        });

        for (unsigned int index = 0; index < size.size(); ++index) {


            connecting_polygon_index = size[index].second;

            polygon_i connecting_polygon = pieces.at(can_connect_polygon_number[connecting_polygon_index]);

            std::vector<polygon_i> output;
            boost::geometry::union_(connect_polygon,connecting_polygon,output);

            auto isPointEqual = [](point_i first,point_i second){
                return first.x() == second.x() && first.y() == second.y();
            };

            //ピースの中に隣り合わず重複した点があると、2つのpolygonに分離できる状態なのに、分離されないヤバイ感じのになるので。ここで除去
            bool flag = false;
            for (unsigned int a = 1; a < output[0].outer().size() - 1; ++a) {
                for (unsigned int b = 1; b < output[0].outer().size() - 1; ++b) {
                    if(a == b){
                        b++;
                    }

                    flag += isPointEqual(output[0].outer()[a],output[0].outer()[b]);
                }
            }

            std::cout << flag << std::endl;

            if(flag){
                if(index == (size.size() - 1)){
                    pieces.clear();
                    std::copy(back_up_polygon.begin(),back_up_polygon.end(),std::back_inserter(pieces));
                    goto ONCE_MORE;
                }
            }else{
                //connectしたわけなので、元からのpolygonは除去
                pieces.erase(pieces.begin() + can_connect_polygon_number[connecting_polygon_index]);
                pieces.push_back(output[0]);
                break;
            }
        }

        return pieces;
    };

    std::vector<polygon_i> pieces;
    std::copy(polygons.begin(),polygons.end(),std::back_inserter(pieces));
    for (int var = 0; var < 140; ++var) {
        std::vector<polygon_i> pieces_buf = connect_near_by_polygon(pieces);
        pieces.clear();
        std::copy(pieces_buf.begin(),pieces_buf.end(),std::back_inserter(pieces));
    }

    this->print_polygons.clear();
    std::copy(pieces.begin(),pieces.end(),std::back_inserter(this->print_polygons));


}

void ProbMaker::step()
{
    //いい感じに乱数でpolygonを切断して
    auto devide_polygon = [](polygon_i polygon)->std::pair<polygon_i,polygon_i>{
        std::random_device rnd;
        std::uniform_int_distribution<> random(0,polygon.outer().size() - 2);

        auto calc_great_common_divisor = [](int a,int b)->int{
            return boost::math::gcd(a,b);
        };

        //切り始める辺と終わりの辺[0~...]
        int start_side = random(rnd);
        int end_side = random(rnd);

        //同じ番号が来られると困るのでこんな感じに違うのが出るまでやり続ける
        while(end_side == start_side){
            end_side = random(rnd);
        }

        //start_side < end_side になるように変更
        if(start_side > end_side){
            int tmp = start_side;
            start_side = end_side;
            end_side = tmp;
        }

        std::vector<point_i> start_side_cut_points;
        std::vector<point_i> end_side_cut_points;

        //(x,y)ともに整数になるパターンを列挙するため傾きを求める
        int start_side_gcd = calc_great_common_divisor(std::abs(polygon.outer().at(start_side + 1).x() - polygon.outer().at(start_side).x()),
                                                         std::abs(polygon.outer().at(start_side + 1).y() - polygon.outer().at(start_side).y()));
        int end_side_gcd = calc_great_common_divisor(std::abs(polygon.outer().at(end_side + 1).x() - polygon.outer().at(end_side).x()),
                                                       std::abs(polygon.outer().at(end_side + 1).y() - polygon.outer().at(end_side).y()));

        int start_dx = (polygon.outer().at(start_side + 1).x() - polygon.outer().at(start_side).x()) / start_side_gcd;
        int start_dy = (polygon.outer().at(start_side + 1).y() - polygon.outer().at(start_side).y()) / start_side_gcd;

        if(start_dx == 0){
            start_dy = start_dy > 0 ? 1 : -1;
        }else if(start_dy == 0){
            start_dx = start_dx > 0 ? 1 : -1;
        }

        int end_dx = (polygon.outer().at(end_side + 1).x() - polygon.outer().at(end_side).x()) / end_side_gcd;
        int end_dy = (polygon.outer().at(end_side + 1).y() - polygon.outer().at(end_side).y()) / end_side_gcd;

        if(end_dx == 0){
            end_dy = end_dy > 0 ? 1 : -1;
        }else if(end_dy == 0){
            end_dx = end_dx > 0 ? 1 : -1;
        }

        int current_x = polygon.outer().at(start_side).x();
        int current_y = polygon.outer().at(start_side).y();

        //開始点になれる可能性のある点を列挙
        while((current_x != polygon.outer().at(start_side + 1).x()) || (current_y != polygon.outer().at(start_side + 1).y())){
            start_side_cut_points.push_back(point_i(current_x,current_y));

            current_x += start_dx;
            current_y += start_dy;
        }
        start_side_cut_points.push_back(point_i(current_x,current_y));

        //終点になれる可能性のある点を列挙
        current_x = polygon.outer().at(end_side).x();
        current_y = polygon.outer().at(end_side).y();
//        std::cout << "current_X:" << current_x << "current_y:" << current_y << std::endl;
//        std::cout << "point_x" << polygon.outer().at(end_side + 1).x() << "point_y" << polygon.outer().at(end_side + 1).y() <<  std::endl;

//        std::cout << (current_x != polygon.outer().at(end_side + 1).x()) << (current_y != polygon.outer().at(end_side + 1).y()) << std::endl;
//        std::cout << ((current_x != polygon.outer().at(end_side + 1).x()) && (current_y != polygon.outer().at(end_side + 1).y())) << std::endl;
//        std::cout << (false && true) << std::endl;

        while((current_x != polygon.outer().at(end_side + 1).x()) || (current_y != polygon.outer().at(end_side + 1).y())){
            end_side_cut_points.push_back(point_i(current_x,current_y));

            current_x += end_dx;
            current_y += end_dy;
        }
        end_side_cut_points.push_back(point_i(current_x,current_y));

//        for(auto point: start_side_cut_points){
//            std::cout << "start:" << "x:" << point.x() << "y" << point.y() << std::endl;
//        }
//        for(auto point: end_side_cut_points){
//            std::cout << "end" << "x:" << point.x() << "y" << point.y() << std::endl;
//        }
        
        //実際に切る点を決める乱数生成
		std::uniform_int_distribution<> start_dot_random(0,start_side_cut_points.size() - 1);
        std::uniform_int_distribution<> end_dot_random(0,end_side_cut_points.size() - 1);
        
        unsigned int start_dot_num = start_dot_random(rnd);
        unsigned int end_dot_num = end_dot_random(rnd);

        //new points vector
        std::vector<point_i> new_points1;
        std::vector<point_i> new_points2;

        unsigned int a = 0;
        while(true){
            //start dot numにたどり着くまでpush_back
            new_points1.push_back(polygon.outer().at(a));
            if(a == start_side){
                break;
            }

            ++a;
        }
        
        //切断店の挿入
        new_points1.push_back(start_side_cut_points.at(start_dot_num));
        new_points2.push_back(start_side_cut_points.at(start_dot_num));

        ++a;
        
        while(true){
            new_points2.push_back(polygon.outer().at(a));
             if(a == end_side){
                break;
            }

            ++a;
        }

        //切断点の挿入
        new_points1.push_back(end_side_cut_points.at(end_dot_num));
        new_points2.push_back(end_side_cut_points.at(end_dot_num));

        ++a;
        
        //new_points2はここで終点なのでindexが0の座標を最後に挿入する
        new_points2.push_back(new_points2.at(0));

        while (true) {
            new_points1.push_back(polygon.outer().at(a));
            if(a == polygon.outer().size() - 1){
                break;
            }

            a++;
        }

        //new_points2はここで終点なのでindexが0の座標を最後に挿入
        new_points1.push_back(new_points1.at(0));

        auto delete_depulicated_point = [](polygon_i polygon)->polygon_i{
          	polygon_i deleted_duplicated_point_polygon;
            auto check_same_point = [](point_i point1,point_i point2)->bool{
                return point1.x() == point2.x() && point1.y() == point2.y();
            };

            std::cout << "debugguggggggasdkljgklajsldkgkla" << std::endl;
            std::cout << check_same_point(point_i(100,99),point_i(100,99)) << std::endl;
            std::cout << check_same_point(point_i(100,100),point_i(100,99)) << std::endl;


            for (unsigned int index = 0; index < polygon.outer().size() - 1; ++index) {
                if(!check_same_point(polygon.outer().at(index),polygon.outer().at(index+1))){
                    deleted_duplicated_point_polygon.outer().push_back(polygon.outer().at(index));
                }
            }
            deleted_duplicated_point_polygon.outer().push_back(polygon.outer().at(polygon.outer().size()-1));
            
        	return deleted_duplicated_point_polygon;
        };

        auto points_to_polygon = [](std::vector<point_i> points)->polygon_i{
            polygon_i return_polygon;
            for(auto point : points){
                return_polygon.outer().push_back(point);
            }
            return return_polygon;
        };

        std::pair<polygon_i,polygon_i> polygons;
        polygons.first = delete_depulicated_point(points_to_polygon(new_points1));
        polygons.second = delete_depulicated_point(points_to_polygon(new_points2));

        return polygons;
    };

    auto find_iikanji_polygon = [&devide_polygon](polygon_i polygon)->std::pair<polygon_i,polygon_i>{
        auto check_polygon = [](std::pair<polygon_i,polygon_i> polygons)->bool{
            //polygonの面積をチェック
            const double first_polygon_area = boost::geometry::area(polygons.first);
            const double second_polygon_area = boost::geometry::area(polygons.second);
            const double total_area = first_polygon_area + second_polygon_area;

            //全体の合計から少なくともこれだけの面積はほしいってやつ
            const double threshold = 0.2;

            if(first_polygon_area > second_polygon_area){
                if(second_polygon_area > total_area * threshold){
                    return true;
                }else{
                    return false;
                }
            }else{
                if(first_polygon_area > total_area * threshold){
                    return true;
                }else{
                    return false;
                }
            }
            return true;
        };

        auto check_inner_angle = [](polygon_i polygon,double degree_threshold){
            auto calc_angle = [](polygon_i polygon,int pos){
                int x1,y1,x2,y2;

                //posが0だと戻らないといけないから
                if(pos == 0){
                    x1 = polygon.outer().at(pos).x() - polygon.outer().at(polygon.outer().size() - 2).x();
                    y1 = polygon.outer().at(pos).y() - polygon.outer().at(polygon.outer().size() - 2).y();
                    x2 = polygon.outer().at(pos).x() - polygon.outer().at(pos + 1).x();
                    y2 = polygon.outer().at(pos).y() - polygon.outer().at(pos + 1).y();
                }else{
                    x1 = polygon.outer().at(pos).x() - polygon.outer().at(pos - 1).x();
                    y1 = polygon.outer().at(pos).y() - polygon.outer().at(pos - 1).y();
                    x2 = polygon.outer().at(pos).x() - polygon.outer().at(pos + 1).x();
                    y2 = polygon.outer().at(pos).y() - polygon.outer().at(pos + 1).y();
                }

                double bb = std::abs(x1 * x2 + y1 * y2) / (std::sqrt(x1 * x1 + y1 * y1) * std::sqrt(x2 * x2 + y2 * y2));
                double angle = std::acos(bb);

                return angle;
            };

            auto radian2dgree = [](double rad){
                return (rad / M_PI) * 180;
            };

//            std::cout << "jpeg slfaj;sd" << std::endl;
//            std::cout << calc_angle(polygon,0) << std::endl;
//            std::cout << radian2dgree(calc_angle(polygon,0)) << std::endl;

            for (unsigned int index = 0; index < polygon.outer().size() - 1; ++index) {
                double degree = radian2dgree(calc_angle(polygon,index));
                std::cout << "kakudooooo" << degree << std::endl;

                if(degree < degree_threshold){
                    return false;
                }
            }
            return true;
        };

        while(true){
            std::pair<polygon_i,polygon_i> polygons_buf;
            polygons_buf = devide_polygon(polygon);

            std::cout << "debugging" << std::endl;

            if(check_polygon(polygons_buf)){
                if(check_inner_angle(polygon,20)){
                    return polygons_buf;
                }
            }
        }
    };
    

    //一番面積の大きいpolygonのindexを持ってくる
    auto find_greatest_area_polygon = [](std::vector<polygon_i> polygons){
        int current_greatest_area_polygon_index = 0;

        for (unsigned int index = 0; index < polygons.size(); ++index) {
            if(boost::geometry::area(polygons.at(index)) > boost::geometry::area(polygons.at(current_greatest_area_polygon_index))){
                current_greatest_area_polygon_index = index;
            }
        }
        return current_greatest_area_polygon_index;
    };

    auto devide_vectorrrrred_polygon = [&find_greatest_area_polygon,&find_iikanji_polygon](std::vector<polygon_i> polygons)->std::vector<polygon_i>{
        unsigned int greatest_area_polygon_index = find_greatest_area_polygon(polygons);
        std::cout << "greatest polygon number:" << greatest_area_polygon_index << std::endl;
        polygon_i polygon_buff;
        polygon_buff = polygons.at(greatest_area_polygon_index);
//        polygons.erase(polygons.begin() + greatest_area_polygon_index);

        std::vector<polygon_i> return_polygons;
//        std::copy(polygons.begin(),polygons.end(),std::back_inserter(return_polygons));
        for (unsigned int index = 0; index < polygons.size(); ++index) {
            if(index != greatest_area_polygon_index){
                return_polygons.push_back(polygons.at(index));
            }
        }

        std::pair<polygon_i,polygon_i> polygons_buf = find_iikanji_polygon(polygon_buff);
        return_polygons.push_back(polygons_buf.first);
        return_polygons.push_back(polygons_buf.second);

        return return_polygons;
    };

    std::pair<polygon_i,polygon_i> devided_polygon = devide_polygon(this->print_polygons[0]);
    std::cout << "oppsdjfklajfksld;nlkvas " << std::endl;
    std::cout << "first" << boost::geometry::dsv(devided_polygon.first) << std::endl;
    std::cout << "second" << boost::geometry::dsv(devided_polygon.second) << std::endl;


//    while(true){
//        std::vector<polygon_i> polygon_buf;

//        std::copy(this->print_polygons.begin(),this->print_polygons.end(),std::back_inserter(polygon_buf));
//		this->print_polygons.clear();

//        auto polygons = find_iikanji_polygon(base_polygon);
//    }
//    this->print_polygons.push_back(polygons.first);
//    this->print_polygons.push_back(polygons.second);

    std::cout << "DEVIDE" << std::endl;

    std::vector<polygon_i> polygonn = devide_vectorrrrred_polygon(this->print_polygons);
    this->print_polygons.clear();
    std::copy(polygonn.begin(),polygonn.end(),std::back_inserter(this->print_polygons));


    int counnnter = 0;
    for(auto polygon: this->print_polygons){
        ++counnnter;
    }

    this->update();

//    QEventLoop event;
//    connect(this,&ProbMaker::nextLoop,&event,&QEventLoop::quit);
//    event.exec();

    

}

std::vector<polygon_i> ProbMaker::getPieces()
{
    return this->print_polygons;
}

polygon_i ProbMaker::getFrame()
{
    return frame;
}

void ProbMaker::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QPen(QColor("#000000")));
    constexpr int grid_size = 10;
    constexpr int max_col = 101;
    constexpr int max_row = 65;
    for (int col= 0; col < max_col; ++col) {
        for(int row = 0; row < max_row; ++row){
            std::vector<QPoint> polygon;
            polygon.push_back(QPoint(col*grid_size,row*grid_size));
            polygon.push_back(QPoint(col*grid_size+grid_size,row*grid_size));
            polygon.push_back(QPoint(col*grid_size+grid_size,row*grid_size+grid_size));
            polygon.push_back(QPoint(col*grid_size,row*grid_size+grid_size));
            polygon.push_back(QPoint(col*grid_size,row*grid_size));

            if((row+col) % 2 == 0)
                painter.setBrush(QBrush(QColor("#fef4f4")));
            else
                painter.setBrush(QBrush(QColor("#e6b422")));
            painter.drawPolygon(&polygon.front(),polygon.size());
        }
    }
    painter.setPen(QPen(QColor("#00ffff")));
    auto draw_polygon = [&painter,&grid_size](polygon_i polygon){
        std::vector<QPoint> print_points;
        for(auto point : polygon.outer()){
            print_points.push_back(QPoint(point.x()*grid_size,point.y()*grid_size));
            painter.drawPoint(point.x(),point.y());
        }
        painter.drawPolygon(&print_points.front(),print_points.size());
    };

    for(auto polygon : this->print_polygons){
        draw_polygon(polygon);
    }

    auto draw_points = [&painter,&grid_size](polygon_i polygon){
        int counter = 0;
        for(auto point : polygon.outer() ){
            painter.drawEllipse(point.x()*grid_size,point.y()*grid_size,10,10);
            painter.drawText(point.x()*grid_size,point.y()*grid_size+ 10,QString(QString::fromStdString(std::to_string(counter))));
            ++counter;
        }
    };

    painter.setPen(QPen(QColor("#000000")));
    for(auto polygon : this->print_polygons){
       draw_points(polygon);
    }
}

void ProbMaker::keyPressEvent(QKeyEvent *event){
    std::cout << event->key() << std::endl;

    //if pressed a key
    if(event->key() == 65){
        std::cout << "wweeeeeee" << std::endl;
        emit nextLoop();
    }
}
