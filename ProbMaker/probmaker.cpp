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

//ここから中野先輩のコード

bool ProbMaker::IsCongruence(polygon_i polygon1 , polygon_i polygon2){

    struct Triangle{
        int line1;
        int line2;
        double angle;
        bool depression;
    };
    auto sumSquare=[](point_i point1 , point_i point2)
    {
        int x_2 = pow(point1.x() - point2.x(),2.0);
        int y_2 = pow(point1.y() - point2.y(),2.0);
        return x_2 + y_2;
    };
    auto computeAngle = [](point_i pointA , point_i pointB , point_i pointC){
       int a1,a2,b1,b2;
       a1=pointA.x() - pointB.x();
       a2=pointA.y() - pointB.y();
       b1=pointC.x() - pointB.x();
       b2=pointC.y() - pointB.y();
       double bunshi = a1 * b1 + a2 * b2;
       double bunbo=sqrt(pow(a1 , 2) + pow(a2 , 2)) * sqrt(pow(b1 , 2) + pow(b2 , 2));
       return acos(bunshi/bunbo);
   };
    auto isDepression = [](polygon_i polygonA,int number){
        point_i removepoint = polygonA.outer().at(number);
        polygonA.outer().erase(polygonA.outer().begin() + number);
        if(number == 0){
            polygonA.outer().erase(polygonA.outer().end() - 1);
            polygonA.outer().push_back(polygonA.outer().at(0));
        }
        return !bg::disjoint(polygonA , removepoint);
    };
    auto getTriangles = [sumSquare , computeAngle , isDepression](polygon_i polygonA){
        std::vector<point_i> polygonAvector = polygonA.outer();
        int serchNumber = polygonAvector.size() - 2;
        std::vector<Triangle> v;
        for(int i=0 ; i <= serchNumber ; i++){
            int a = i - 1 , b = i , c = i + 1;
            if(a==-1) a = serchNumber;
            v.push_back(
                {
                    sumSquare(polygonAvector.at(a) , polygonAvector.at(b)),
                    sumSquare(polygonAvector.at(b) , polygonAvector.at(c)),
                    computeAngle(polygonAvector.at(a) , polygonAvector.at(b) , polygonAvector.at(c)),
                    isDepression(polygonA , b)
                }
            );
        }
        return v;
    };
    auto equalsTriangles1 = [](Triangle triangleA,Triangle triangleB){
        bool a = triangleA.line1 == triangleB.line1;
        bool b = triangleA.line2 == triangleB.line2;
        bool c = triangleA.angle == triangleB.angle;
        bool d = triangleA.depression == triangleB.depression;
        return a && b && c && d;
    };
    auto equalsTriangles2=[](Triangle triangleA,Triangle triangleB){
        bool a = triangleA.line1 == triangleB.line2;
        bool b = triangleA.line2 == triangleB.line1;
        bool c = triangleA.angle == triangleB.angle;
        bool d = triangleA.depression == triangleB.depression;
        return a && b && c && d;
    };
    auto isThereTriangle = [equalsTriangles1 , equalsTriangles2](Triangle triangle , std::vector<Triangle> vectorA){
        std::vector<std::pair<int , bool>> v;
        int size = vectorA.size();
        for(int i = 0 ; i < size ; i++){
            bool et1 = equalsTriangles1(triangle , vectorA.at(i));
            bool et2 = equalsTriangles2(triangle , vectorA.at(i));
            if(et1||et2) v.push_back(std::pair<int , bool>(i , et1));
        }
        return v;
    };
    auto equalsVector = [equalsTriangles1,equalsTriangles2](std::vector<Triangle> vectorA , std::vector<Triangle> vectorB , bool forward){
        if(vectorA.size() != vectorB.size()) return false;
        int size = vectorA.size();
        for(int i = 0 ; i < size ; i++){
            if(forward){
                if(!equalsTriangles1(vectorA.at(i),vectorB.at(i))) return false;
            }else{
                if(!equalsTriangles2(vectorA.at(i),vectorB.at(size-i-1))) return false;
            }
        }
        return true;
    };
    auto equalsTriangleVectors = [equalsTriangles1 , equalsTriangles2 , isThereTriangle , equalsVector](std::vector<Triangle> vectorA , std::vector<Triangle> vectorB){
        if(vectorA.size() != vectorB.size()) return false;
        std::vector<std::pair<int , bool>> v = isThereTriangle(vectorA.at(0) , vectorB);
        for(int i=0 ; i < v.size() ; i++){
            std::pair<int , bool> intandbool = v.at(i);
            int a = intandbool.first;
            bool b = intandbool.second;
            if(!b) a++;
            for(int j = 0 ; j < a ; j++){
                Triangle temporary = vectorB.at(0);
                vectorB.erase(vectorB.begin());
                vectorB.push_back(temporary);
            }
            if(equalsVector(vectorA , vectorB , b)) return true;
        }
        return false;
    };
    std::vector<Triangle> v1 = getTriangles(polygon1);
    std::vector<Triangle> v2 = getTriangles(polygon2);
    bool answer = equalsTriangleVectors(v1 , v2);
    return answer;
}
//ここまで中野先輩のコード　マージしたら消す





ProbMaker::~ProbMaker()
{
    delete ui;
}


int ProbMaker::retRnd(int num){
    std::random_device rand;
    std::mt19937 mt(rand());
    std::uniform_int_distribution<int> rnd(0,num-1);//randomを返すだけ
    return rnd(mt);
}

void ProbMaker::angulated_graphic(){
    int tescou = -1;
    bool flag;
    do{
    print_polygons.clear();
    //frame = sample_frame;

    createFrame();
    if(!usePieceDataToFrame)std::cout << "枠生成完了　" << std::endl;


    check_frame = frame;//すでにはめられたピースも含むpolygonを生成する

    setInnerFrame(frame);// 初期状態のFrameをInnerFrameに投入
    for(int count = 0;count<200;++count){
        createPiece(check_frame);//どうやらここでcreatePieceを無限実行してるのが不具合になってたみたい 上限を設けて不具合を回避した
        if(bg::area(check_frame) < 200 || print_polygons.size() > 49)break;

    }
    //枠の残り部分をそのままピースとして出力する
    print_polygons.push_back(check_frame);
    std::cout << "ピース生成完了" << std::endl;

    //大きいのを分割
    if(splitPiece()){
        std::cout << "ピース分割完了" << std::endl;
        //小さなピースの結合
        jointPiece();//この関数内でSEGV起こしてる
        std::cout << "ピース結合完了" << std::endl;


        flag=false;
        for(auto poly : print_polygons){
            if(bg::area(poly) > bg::area(frame) / 17){flag=true;//ここでピースの大きさの最大値を設定している
            std::cout << "ピースの大きさが限界値を超えています" << std::endl;}
        }
        if(congruenceCheck())flag=true;
        std::cout << "問題検知終了" << std::endl;

    }else flag=true;//falseが返されたなら分割ができていないためやり直し
    tescou++;

    if(flag)std::cout << "問題があったのでやり直し" << std::endl;
    }while(flag);

    erasePoint();

    if(usePieceDataToFrame)createFrameFromPiece();
    /*
    for(auto polygon : print_polygons){//生成されたポリゴンの一覧を出力する
        std::cout << "polygon = " << bg::dsv(polygon) << std::endl;
        std::cout << "area = " << bg::area(polygon) << std::endl << std::endl;
    }
    std::cout << "piece area = " << bg::area(frame) << std::endl;
    std::cout << "piece count = " << print_polygons.size() << std::endl;
    std::cout << "piece area average = " << bg::area(frame) / print_polygons.size() << std::endl;
    */
    if(tescou)std::cout << "やり直されています　回数 : " << tescou << std::endl;
}


void ProbMaker::createFrame(){//枠の生成　const定数で挙動の変更をしている

    frame.clear();
    if(!usePieceDataToFrame){

    int firstcoordinate_x,firstcoordinate_y,leftline,overline,rightline,underline;
    do{
    leftline = retRnd(15),overline = retRnd(10),rightline = 86+retRnd(15),underline = 56+retRnd(10);

    std::vector<int> vertex1;

        bg::clear(real_frame);
    for(int a = 0;a < 4;a++){
         vertex1.push_back(retRnd(4));
    }
    int inter;
    std::set<int> anaraiz(vertex1.begin(),vertex1.end());
    std::vector<int> vertex(anaraiz.begin(),anaraiz.end());
    int count0 = 0,count1 = 0,count2 = 0,count3 = 0;
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
    inter = retRnd(22);
    if(count0 == 1){
        real_frame.outer().push_back(point_i(inter+leftline,overline));
        real_frame.outer().push_back(point_i(leftline,inter+overline));
        firstcoordinate_x = inter + leftline,firstcoordinate_y = overline;
    }else{
        real_frame.outer().push_back(point_i(leftline,overline));
        firstcoordinate_x = leftline,firstcoordinate_y = overline;
    }
    inter = retRnd(22);
    if(count1 == 1){
        real_frame.outer().push_back(point_i(leftline,underline-inter));
        real_frame.outer().push_back(point_i(inter+leftline,underline));
    }else{
        real_frame.outer().push_back(point_i(leftline,underline));
    }
    inter = retRnd(22);
    if(count2 == 1){
        real_frame.outer().push_back(point_i(rightline-inter,underline));
        real_frame.outer().push_back(point_i(rightline,underline-inter));
    }else{
        real_frame.outer().push_back(point_i(rightline,underline));
    }
    inter = retRnd(22);
    if(count3 == 1){
        real_frame.outer().push_back(point_i(rightline,inter+overline));
        real_frame.outer().push_back(point_i(rightline-inter,overline));
    }else{
        real_frame.outer().push_back(point_i(rightline,overline));
    }
    real_frame.outer().push_back(point_i(firstcoordinate_x,firstcoordinate_y));
}while(bg::area(real_frame) < frame_size);
    frame = real_frame;

    }else{

        frame.outer().push_back(point_i(0,0));
        frame.outer().push_back(point_i(0,65));
        frame.outer().push_back(point_i(101,65));
        frame.outer().push_back(point_i(101,0));
        frame.outer().push_back(point_i(0,0));
        bg::correct(frame);

    }
}

void ProbMaker::createFrameFromPiece(){
    setInnerFrame(frame);
    while(bg::area(frame) > frame_size + 800){
    for(unsigned int count = 0;count < print_polygons.size();++count){//要素そのものを削除する都合上for eachは使わない方向で
        //setInnerFrame(frame);
        if(bg::intersects(inner_frame,print_polygons.at(count)) && !retRnd(8)){//frameと接触している時に一定確率で
            std::vector<polygon_i> differ_frame;
            bg::difference(frame,print_polygons.at(count),differ_frame);
            if(differ_frame.size() == 1){
                bg::correct(differ_frame.at(0));
                print_polygons.erase(print_polygons.begin() + count);
                frame = differ_frame.at(0);
                break;
            }
        }
    }
    }
}

bool ProbMaker::congruenceCheck(){

    for(unsigned int poly_num =0;poly_num<print_polygons.size();poly_num++){//汚いけどfor eachじゃないやつでやる
        for(unsigned int check_poly_num=poly_num+1;check_poly_num<print_polygons.size();check_poly_num++){
            if(IsCongruence(print_polygons[poly_num] , print_polygons[check_poly_num]))return true;
        }
    }

    return false;//問題がなければfalseを返して終了

}

void ProbMaker::erasePoint(){//直線上にある(消しても問題ない)頂点を削除

    for(auto& poly : print_polygons){
        bool flag;
        bg::unique(poly);
        do{

        flag=false;

        std::vector<point_i> vec_points;//図形の頂点をこれに格納
        for(auto point : poly.outer()){
            vec_points.push_back(point);
        }
        vec_points.push_back(poly.outer().at(1));

        for(unsigned int point_cou=0;point_cou < vec_points.size() - 2;++point_cou){
            bg::model::linestring<point_i> check_line;//二点で線を作り、それが点と接触しているかを判定する
            check_line.push_back(vec_points.at(point_cou));
            check_line.push_back(vec_points.at(point_cou + 2));

            if(bg::intersects(check_line,vec_points.at(point_cou + 1))){//接触判定

                vec_points.erase(vec_points.begin() + point_cou + 1);
                vec_points.erase(vec_points.begin());//ここ消さないとcorrect時に削除した頂点がくっついちゃってヤバい
                bg::clear(poly);
                for(unsigned int point_number = 0;point_number < vec_points.size() - 1;++point_number){
                    poly.outer().push_back(vec_points.at(point_number));
                }
                flag=true;
                break;
            }
        }

        bg::unique(poly);
        bg::correct(poly);
        }while(flag);
    }
}

bool ProbMaker::splitPiece(){

    bool flag;
    for(int count=0;count < 15;count++){//分割できないパターンでの無限ループ防止
    flag=true;
    for(unsigned int poly_num =0;poly_num<print_polygons.size();++poly_num){//for eachから変更したら問題を起こさなくなった
        if(bg::area(print_polygons[poly_num]) > 150){//ピースの大きさが一定を超えているなら
            if(onlySplitRightAngle) createPiece(print_polygons[poly_num]);
            else splitDiagonally(print_polygons[poly_num]);
            flag=false;
        }
        if(print_polygons.size() > 59)break;//ピース数が50を超えないように調整
    }
    if(print_polygons.size() > 59)break;
    for(auto poly : print_polygons){
        if(bg::area(poly) > 150) flag = false;
    }
    if(flag)break;
    }
    for(auto poly : print_polygons){
        if(bg::area(poly) > bg::area(frame) / 5) return false;
    }
        return true;
}

void ProbMaker::splitDiagonally(polygon_i& poly){
    bool flag=false;
    bool check;
    setInnerFrame(poly);
    point_i closs_point;
    do{
        check=false;
        closs_point = returnClossPoint(poly);
        for(auto point : poly.outer()){
            if(bg::equals(closs_point,point))check=true;
        }
    }while(check);
    bool left_or_right = retRnd(2);
    std::vector<point_i> vec_points;//図形の頂点をこれに格納
    for(auto point : poly.outer()){
        vec_points.push_back(point);
    }
    vec_points.push_back(poly.outer().at(1));
    bool change_side = false;
    for(int count=0;count<2;count++){//右下方向、左下方向へ二回繰り返される
        for(int extend = 1;extend < 65;extend++){
            int extend_x = (left_or_right
                                   ? closs_point.x()+extend
                                   : closs_point.x()-extend
                                   );
            int extend_y = closs_point.y()+extend;
            point_i extend_point = point_i(extend_x,extend_y);

            for(unsigned int point_cou=0;point_cou < vec_points.size() - 2;++point_cou){
                bg::model::linestring<point_i> check_line;//二点で線を作り、それが点と接触しているかを判定する
                check_line.push_back(vec_points.at(point_cou));
                check_line.push_back(vec_points.at(point_cou + 1));

                if(bg::intersects(check_line,extend_point)){//接触判定
                    if(extend==1){
                        change_side = true;
                        break;
                    }
                    polygon_i split_poly;
                    split_poly.outer().push_back(closs_point);
                    split_poly.outer().push_back(extend_point);
                    checkClossLine(split_poly,poly);//ここでSEGV 交点はちゃんと出せてるのだが…　　交点が丁度頂点の位置になっていたり交点と平行になっていたりするっぽい…？　　　ランダムで選んだ始点がチェックする線の交点と重なってるぞ！！！
                    print_polygons.push_back(split_poly);
                    flag=true;
                    break;
                }
            }
            if(flag || change_side)break;
        }
        if(flag)break;
        left_or_right ^= 1;
    }
}

void ProbMaker::jointPiece(){

    bool flag = false;
    for(int count=0;count < 30;count++){
        bool check = false;
        int piece_cou = 0;//結合元のpieceの番号
        for(auto poly : print_polygons){
            if(check)break;
            int check_cou = 0;//結合先のpieceの番号
            for(auto check_poly : print_polygons){
                if(bg::area(poly) < 40 && bg::intersects(poly,check_poly) && !bg::equals(poly,check_poly)){//面積が一定以下で他のピースと隣接していたなら結合

                    std::vector<polygon_i> union_poly;
                    bg::union_(poly,check_poly,union_poly);
                    if(!bg::num_interior_rings(union_poly[0]) && union_poly.size() == 1 ){//結合後にinnerが存在してしまうようなら結合しない
                        if(bg::area(union_poly[0]) < 400 || count == 29 ){
                            print_polygons[piece_cou] = union_poly[0];
                            print_polygons.erase(print_polygons.begin() + check_cou);
                            check=true;
                            break;
                        }
                    }
                }
                ++check_cou;
            }
            ++piece_cou;
        }
        flag = true;
        for(auto poly : print_polygons){
            if(bg::area(poly) < 30) flag = false;// areaが一定以下のピースが存在するならもう一度繰り返す
        }
        if(flag)break;
    }
}

void ProbMaker::setInnerFrame(polygon_i frame){
    inner_frame.clear(); //中身をリセットしてからcheck_frameのデータを取得する
    inner_frame.outer().push_back(point_i(0,0));
    inner_frame.outer().push_back(point_i(101,0));
    inner_frame.outer().push_back(point_i(101,65));
    inner_frame.outer().push_back(point_i(0,65));
    inner_frame.outer().push_back(point_i(0,0));
    inner_frame.inners().push_back(polygon_i::ring_type());
    bg::correct(inner_frame);
    for(auto point : frame.outer()){
       inner_frame.inners().back().push_back(point);
    }
}

point_i ProbMaker::returnClossPoint(polygon_i poly){
    bool check=false;
    int point_y,point_x;
    while(!check){
        point_x = 1 + retRnd(99);//ランダムでx座標を出す
        for(int closspointy=0;closspointy<66;closspointy++){//縦に引かれた線と枠の線の交点を出す
            if( bg::intersects(point_i(point_x,closspointy), poly)){
                point_y = closspointy; //交点のy座標を記憶
                check = true;
                break;
            }
        }
    }
    return point_i(point_x,point_y);
}

void ProbMaker::createPiece(polygon_i& argument_frame){//引数には枠を指定する
    //ここから関数
    polygon_i poly;
    int point_y,point_x;
    setInnerFrame(argument_frame);

    point_i closs_point = returnClossPoint(argument_frame);
    poly.outer().push_back(closs_point);

    point_x = closs_point.x();
    point_y = closs_point.y();

    bool x_or_y = true;//次にx軸方向へ伸ばすかy軸方向に伸ばすかを記録する trueならy軸方向、falseならx軸方向に伸ばす
    //次はx座標に正の方向へ頂点を移動させるサンプルを作ってみる

    bool flag = false;
    bool point_pushback;
    int cou=0;

    while(!flag){//他の枠やピース、自分の線とぶつかったら終了するようにする
        cou++;

        bool add_or_subt = retRnd(2);
        point_pushback = true; // 頂点をpush_backするか決める変数　これがfalseなら点をpush_backせずやり直す
        if(add_or_subt){
            if(point_x != 101 && point_y != 65){
                int extend = (x_or_y
                            ? retRnd(15) + 6
                            : retRnd(20) + 6
                            );//とりあえず1/3を上限に線を伸ばす
                for(int extend_=1;extend_<extend + 1;extend_++){//図形と接触するかを確認するためのループ
                    if(!x_or_y) ++point_x;
                    else  ++point_y;

                    if(bg::intersects( point_i(point_x,point_y), inner_frame) || bg::intersects(poly,point_i(point_x,point_y))){//これで接触した部分の座標がわかる
                        if(bg::num_points(poly) > 1 && !bg::intersects(poly,point_i(point_x,point_y))){
                            flag = true;
                        }else{
                            point_pushback = false;//始点の直後で失敗したらpush_backせずにやり直す
                            if(!x_or_y) point_x -= extend_;
                            else point_y -= extend_;
                            if(cou>100)flag = true;//無限ループ防止のアレ
                        }
                        break;//他の図形と接触したらそこで止める
                    }
                }
            }else point_pushback = false; //端だったならここの処理を行う
        }else{
            if(point_x != 0 && point_y != 0){//point_x(y)が両方0でなければ
                int extend = (x_or_y
                             ? retRnd(15) + 6
                             : retRnd(20) + 6
                         );//とりあえず1/3を上限に線を伸ばす
                for(int extend_=1;extend_<extend + 1;extend_++){//図形と接触するかを確認するためのループ
                    if(!x_or_y) --point_x;
                    else  --point_y;
                    if(bg::intersects( point_i(point_x,point_y), inner_frame) || bg::intersects(poly,point_i(point_x,point_y))){//これで接触した部分の座標がわかる
                        if(bg::num_points(poly) > 1 && !bg::intersects(poly,point_i(point_x,point_y))){
                            flag = true;
                        }else{
                            point_pushback = false;//始点の直後で失敗したらpush_backせずにやり直す
                            if(!x_or_y) point_x += extend_;
                            else  point_y += extend_;
                            if(cou>100)flag = true;//無限ループ防止のアレ
                        }
                        break;//他の図形と接触したらそこで止める
                    }
                }
            }else point_pushback = false; //端だったならここの処理を行う
        }
        if(point_pushback){
            poly.outer().push_back(point_i(point_x , point_y)); //頂点を確定させる
            x_or_y ^= 1;//次の実行時に向きを変えるようにする(xに進めるかyに進めるかを決める)
        }
    }

    if(point_pushback){
            checkClossLine(poly , argument_frame);
            print_polygons.push_back(poly);
    }
}





void ProbMaker::checkClossLine(polygon_i& poly , polygon_i& change_frame){//polyは渡された時点で線、change_frameはpolyに合わせて変更するpolygon
    int begin_line = -1;//変数名のせいでわかりづらいけど枠やピースの交点の線の番号を表している
    int end_line = -1;
    point_i polygon_begin = poly.outer().at(0);
    point_i polygon_end = poly.outer().at(bg::num_points(poly)-1);//polygonの始点と終点(他の枠やピースと繋がってる部分)
    for(unsigned int linenum=0;linenum<bg::num_points(change_frame)-1;linenum++){

        bg::model::linestring<point_i> edge_line{change_frame.outer().at(linenum),change_frame.outer().at(linenum+1)};
        if(bg::intersects(polygon_begin,edge_line))begin_line = linenum;//交点を見つけたら番号を記録
        if(bg::intersects(polygon_end,edge_line))end_line = linenum;
    }
    polygon_i pattern_one = poly;
    polygon_i pattern_two = poly;
    if(begin_line == end_line){//同じ線上にbeginとendがある場合
        pattern_one.outer().push_back(poly.outer().at(0));

        poly.outer().push_back(change_frame.outer().at(end_line+1));
        if(bg::intersects(poly)){

            for(int point_num = end_line;point_num > -1;point_num--){//すごい頭の悪い書き方してる
                pattern_two.outer().push_back(change_frame.outer().at(point_num));
            }
            for(int point_num = bg::num_points(change_frame) - 1;point_num > begin_line;point_num--){
                pattern_two.outer().push_back(change_frame.outer().at(point_num));
            }
            pattern_two.outer().push_back(poly.outer().at(0));

        }else{
            for(unsigned int point_num=end_line + 1;point_num<bg::num_points(change_frame);point_num++){
                pattern_two.outer().push_back(change_frame.outer().at(point_num));
            }
            for(int point_num = 0;point_num < begin_line + 1;point_num++){
                pattern_two.outer().push_back(change_frame.outer().at(point_num));
            }
            pattern_two.outer().push_back(poly.outer().at(0));

        }
        poly.outer().pop_back();
    }else if(begin_line > end_line){
        for(int point_num = end_line + 1;point_num < begin_line + 1;point_num++){
            pattern_one.outer().push_back(change_frame.outer().at(point_num));
        }
        pattern_one.outer().push_back(poly.outer().at(0));

        for(int point_num = end_line;point_num > -1;point_num--){
            pattern_two.outer().push_back(change_frame.outer().at(point_num));
        }
        for(int point_num = bg::num_points(change_frame)-1;point_num > begin_line;point_num--){
            pattern_two.outer().push_back(change_frame.outer().at(point_num));
        }
        pattern_two.outer().push_back(poly.outer().at(0));

    }else{
        for(unsigned int point_num=end_line + 1;point_num<bg::num_points(change_frame);point_num++){
            pattern_one.outer().push_back(change_frame.outer().at(point_num));
        }
        for(int point_num = 0;point_num < begin_line + 1;point_num++){
            pattern_one.outer().push_back(change_frame.outer().at(point_num));
        }
        pattern_one.outer().push_back(poly.outer().at(0));

        for(int point_num = end_line;point_num > begin_line;point_num--){
            pattern_two.outer().push_back(change_frame.outer().at(point_num));
        }
        pattern_two.outer().push_back(poly.outer().at(0));

    }
    poly = pattern_one;
    bg::unique(pattern_one);
    bg::unique(pattern_two);
    bg::correct(pattern_one);//頂点の順番がおかしかった場合は修正
    bg::correct(pattern_two);
    if(bg::area(pattern_one) < bg::area(pattern_two)) poly = pattern_one;//polyに結果を代入する
    else poly = pattern_two;
    //differenceで異なる部分(重複していない部分)を取り出す
    //それをchange_frameに格納する
    bg::correct(change_frame);
    std::vector<polygon_i> differences;
    bg::difference(change_frame,poly,differences);
    change_frame.clear();
    change_frame = differences[0];

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

    auto delete_sample_slope_side = [](polygon_i & p){

        auto calc_slope = [](point_i a,point_i b){
            std::pair<int,int> slope;
            slope.first = boost::math::gcd(std::abs(b.x()-a.x()),std::abs(b.y()-a.y()));
//            slope.second = boos
        };
    };


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
