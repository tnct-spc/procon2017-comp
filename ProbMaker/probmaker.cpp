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
    return rnd(mt);
}

void ProbMaker::angulated_graphic(){

    polygon_i sample_frame;//   テストで枠を生成
    sample_frame.outer().push_back(point_i(12,0));
    sample_frame.outer().push_back(point_i(80,0));
    sample_frame.outer().push_back(point_i(101,21));
    sample_frame.outer().push_back(point_i(101,65));
    sample_frame.outer().push_back(point_i(13,65));
    sample_frame.outer().push_back(point_i(0,52));
    sample_frame.outer().push_back(point_i(0,12));
    sample_frame.outer().push_back(point_i(12,0));
    bg::correct(sample_frame);
    frame = sample_frame;

    check_frame = frame;//すでにはめられたピースも含むpolygonを生成する

    setInnerFrame();// 初期状態のFrameをInnerFrameに投入
    while(1){
        createPiece(check_frame,true);
        if(bg::area(check_frame) < 700 || print_polygons.size() > 48){
            //枠の残り部分をそのままピースとして出力する
            print_polygons.push_back(check_frame);
            break;//ピースを作成するループから抜け出す
        }
    }

    jointPiece();


    //分割するなにがし
    splitPiece();


    for(auto polygon : print_polygons){//生成されたポリゴンの一覧を出力する
        std::cout << "polygon = " << bg::dsv(polygon) << std::endl;
        std::cout << "area = " << bg::area(polygon) << std::endl << std::endl;
    }
    std::cout << "piece area = " << bg::area(frame) << std::endl;
    std::cout << "piece count = " << print_polygons.size() << std::endl;
    std::cout << "piece area average = " << bg::area(frame) / print_polygons.size() << std::endl;


}

void ProbMaker::splitPiece(){
    for(auto poly : print_polygons){
        if(bg::area(poly) > 800){
            createPiece(poly,false);
        }
    }

}

void ProbMaker::jointPiece(){

   /* bool flag = false;
    while(!flag){//flagがtrueにならない限りループ
        for(unsigned int poly_num=0;poly_num<print_polygons.size();++poly_num){
            if(poly_num == print_polygons.size() - 1)flag = true;
            if(bg::area(print_polygons[poly_num]) < 100){//面積が小さい場合
                //隣接するpolygonを探す
                for(unsigned int check_num=0;check_num<print_polygons.size();++check_num){
                    if(bg::touches(print_polygons[poly_num] , print_polygons[check_num]) && poly_num!=check_num ){
                        std::vector<polygon_i> union_poly;
                        bg::union_(print_polygons[poly_num],print_polygons[check_num],union_poly);//結合

                        if(!bg::num_interior_rings(union_poly[0]) && union_poly.size()==1){//innerが存在せず、unionが失敗してないなら
                            print_polygons[poly_num] = union_poly[0];//結合した物を代入
                            print_polygons.erase(print_polygons.begin() + check_num);//結合されたpolygonを削除
                        }
                        //innerが存在する場合は処理を行わないようにする
                    }
                    bg::unique(print_polygons[poly_num]);//線上の重なった点を削除(削除できてないので適当な関数を見つけて改善したい)
                }
                break;//forループを抜け出しまた最初から判定し始める
            }
        }
    }*/
    std::cout << "start joint mode" << std::endl;
    bool flag = false;
    while(!flag){
        bool check = false;
        int piece_cou = 0;//結合元のpieceの番号
        for(auto poly : print_polygons){
            if(check)break;
            int check_cou = 0;//結合先のpieceの番号
            for(auto check_poly : print_polygons){
                if(bg::area(poly) < 100 && bg::touches(poly,check_poly) && piece_cou != check_cou){
                    std::vector<polygon_i> union_poly;
                    bg::union_(poly,check_poly,union_poly);
                    if(!bg::num_interior_rings(union_poly[0]) && union_poly.size() == 1){
                        print_polygons[piece_cou] = union_poly[0];
                        print_polygons.erase(print_polygons.begin() + check_cou);
                        check=true;
                        break;
                    }
                }
                ++check_cou;
            }
            ++piece_cou;
        }
        flag = true;
        for(auto poly : print_polygons){
            if(bg::area(poly) < 100) flag = false;// areaが一定以下のピースが存在するならもう一度繰り返す
        }
    }
}

void ProbMaker::setInnerFrame(){
    inner_frame.clear(); //中身をリセットしてからcheck_frameのデータを取得する
    inner_frame.outer().push_back(point_i(0,0));
    inner_frame.outer().push_back(point_i(101,0));
    inner_frame.outer().push_back(point_i(101,65));
    inner_frame.outer().push_back(point_i(0,65));
    inner_frame.outer().push_back(point_i(0,0));
    inner_frame.inners().push_back(polygon_i::ring_type());
    bg::correct(inner_frame);
    for(auto point : check_frame.outer()){
       inner_frame.inners().back().push_back(point);
    }
}

void ProbMaker::createPiece(polygon_i argument_frame , bool isbeforepiece){//引数には枠、大きさ調整前のピースかを指定する
    //ここから関数
    polygon_i poly;

    bool flag = false;
    int point_y,point_x;
    while(!flag){
    point_x = 1 + retRnd(99);//ランダムでx座標を出す
    for(int closspointy=0;closspointy<66;closspointy++){//縦に引かれた線と枠の線の交点を出す
        if( bg::intersects(point_i(point_x,closspointy), argument_frame)){
            if(!bg::within(point_i(point_x,closspointy + 1) , argument_frame))break;//記憶せずにもう一度
            point_y = closspointy; //交点のy座標を記憶
            flag = true;
            break;
        }
    }
    }
    poly.outer().push_back(point_i(point_x,point_y));


    bool x_or_y = true;//次にx軸方向へ伸ばすかy軸方向に伸ばすかを記録する trueならy軸方向、falseならx軸方向に伸ばす
    //次はx座標に正の方向へ頂点を移動させるサンプルを作ってみる

    flag = false;//flag変数を使いまわしてるけど特に意味はないです
    bool point_pushback;
    int cou=0;

    while(!flag){//他の枠やピース、自分の線とぶつかったら終了するようにする
        cou++;
        bool add_or_subt = retRnd(2);//retRnd(2); // retRnd(2); //trueなら数値を加算、falseなら減算(原点方向へ移動)させる
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
        checkClossLine(poly , isbeforepiece);//ここでpolygonの始点と終点を補完する
        if(isbeforepiece){
            print_polygons.push_back(poly);
            setInnerFrame();// frameをinnerframeに投入
        }else{

        }
    }

}





void ProbMaker::checkClossLine(polygon_i& poly , bool change_check_frame){
    int begin_line = -1;//変数名のせいでわかりづらいけど枠やピースの交点の線の番号を表している
    int end_line = -1;
    point_i polygon_begin = poly.outer().at(0);
    point_i polygon_end = poly.outer().at(bg::num_points(poly)-1);//polygonの始点と終点(他の枠やピースと繋がってる部分)
    for(unsigned int linenum=0;linenum<bg::num_points(check_frame)-1;linenum++){

        bg::model::linestring<point_i> edge_line{check_frame.outer().at(linenum),check_frame.outer().at(linenum+1)};
        if(bg::intersects(polygon_begin,edge_line))begin_line = linenum;//交点を見つけたら番号を記録
        if(bg::intersects(polygon_end,edge_line))end_line = linenum;
    }
    std::cout << "closspoint1 : " << begin_line << "  closspoint2 : " << end_line << std::endl;
    polygon_i pattern_one = poly;
    polygon_i pattern_two = poly;
    if(begin_line == end_line){//同じ線上にbeginとendがある場合
        pattern_one.outer().push_back(poly.outer().at(0));

        poly.outer().push_back(check_frame.outer().at(end_line+1));
        if(bg::intersects(poly)){

            for(int point_num = end_line;point_num > -1;point_num--){//すごい頭の悪い書き方してる
                pattern_two.outer().push_back(check_frame.outer().at(point_num));
            }
            for(int point_num = bg::num_points(check_frame) - 1;point_num > begin_line;point_num--){
                pattern_two.outer().push_back(check_frame.outer().at(point_num));
            }
            pattern_two.outer().push_back(poly.outer().at(0));

        }else{
            for(unsigned int point_num=end_line + 1;point_num<bg::num_points(check_frame);point_num++){
                pattern_two.outer().push_back(check_frame.outer().at(point_num));
            }
            for(int point_num = 0;point_num < begin_line + 1;point_num++){
                pattern_two.outer().push_back(check_frame.outer().at(point_num));
            }
            pattern_two.outer().push_back(poly.outer().at(0));

        }
        poly.outer().pop_back();
    }else if(begin_line > end_line){
        for(int point_num = end_line + 1;point_num < begin_line + 1;point_num++){
            pattern_one.outer().push_back(check_frame.outer().at(point_num));
        }
        pattern_one.outer().push_back(poly.outer().at(0));

        for(int point_num = end_line;point_num > -1;point_num--){
            pattern_two.outer().push_back(check_frame.outer().at(point_num));
        }
        for(int point_num = bg::num_points(check_frame)-1;point_num > begin_line;point_num--){
            pattern_two.outer().push_back(check_frame.outer().at(point_num));
        }
        pattern_two.outer().push_back(poly.outer().at(0));

    }else{
        for(unsigned int point_num=end_line + 1;point_num<bg::num_points(check_frame);point_num++){
            pattern_one.outer().push_back(check_frame.outer().at(point_num));
        }
        for(int point_num = 0;point_num < begin_line + 1;point_num++){
            pattern_one.outer().push_back(check_frame.outer().at(point_num));
        }
        pattern_one.outer().push_back(poly.outer().at(0));

        for(int point_num = end_line;point_num > begin_line;point_num--){
            pattern_two.outer().push_back(check_frame.outer().at(point_num));
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
    //ここからcheck_frameの加工
    //intersectionで重複部分を取り出す
    //differenceで異なる部分(重複していない部分)を取り出す
    //それをcheck_frameに格納する
   bg::correct(check_frame);

   std::cout << " poly = " << bg::dsv(poly) << std::endl;
   std::cout << "check_frame = " << bg::dsv(check_frame) << std::endl;
   std::cout << "inner_frame = " << bg::dsv(inner_frame) << std::endl;

   if(change_check_frame){

    std::vector<polygon_i> differences;
    bg::difference(check_frame,poly,differences);
    std::cout << "difference_size = " << differences.size() << std::endl;
    check_frame.clear();
    for(unsigned int count=0;count<differences.size();count++){
       std::vector<polygon_i> polygon;
       std::cout << " difference = " << bg::dsv(differences[count]) << std::endl;
       bg::union_(check_frame,differences[count],polygon);
       check_frame = polygon[0];
    }
    std::cout << " check_frame = " << bg::dsv(check_frame) << std::endl;

   }

}


/*
//面積が800超えた場合に分割する
flag = false;
bool check = true;
while(check){
    int polygon_num = 0;
    for(auto& polygon : print_polygons){//面積が800以上のピースが複数あっても一度しか処理されてないです        面積の問題が解決したらそこも直したい

        if(bg::area(polygon) > 800){
            //ランダムに線を引く
            //そこの交点二つを選択して分割する
            //面積が偏りすぎてたらやり直し
            while(!flag){
                bool redo = false;//trueならやり直し
                std::vector<int> point_vec;//y(x)座標を格納する
                std::vector<int> line_vec;//接触したpolygonの辺の番号を格納する
                int begin_line = 100;
                int end_line = 100;
                int begin_point = 100;
                int end_point = 100;

                bool x_or_y = true;// retRnd(2);//trueならy軸方向
                int line;
                bg::model::linestring<point_i> check_line;
                do{
                line = 1 + retRnd(100);
                bg::clear(check_line);
                bg::append(check_line,point_i(line,0));
                bg::append(check_line,point_i(line,65));
                std::cout << "line : " << bg::dsv(check_line) << " rndx : " << line << std::endl;
                std::cout << bg::disjoint(check_line , polygon) << std::endl;
                }while(bg::disjoint(check_line,polygon));

                if(x_or_y){

                    for(unsigned int linenum = 0;linenum < print_polygons.size() - 1;linenum++){//polygonの辺の番号       ここのforループ内で問題が起きているのは確定的に明らか
                        bg::model::linestring<point_i> edge_line;//polygonのlinenum番目の辺
                        bg::append(edge_line,polygon.outer().at(linenum));
                        bg::append(edge_line,polygon.outer().at(linenum+1));
                        bool before = false;//一つ前のy座標の地点が辺と接触していたかを記憶する関数
                        for(int y=0;y<65;++y){
                            bool checkline = true;
                            for(auto line : line_vec){
                                if(linenum >= line -1 && linenum <= line+1 )checkline = false;
                            }
                            std::cout << bg::dsv(point_i(line,y)) << bg::dsv(edge_line) << bg::disjoint(point_i(line,y) , edge_line) << std::endl;

                            if(bg::intersects(edge_line, point_i(line,y)) && checkline){//こ↑こ↓の接触判定がガバガバ
                                if(point_vec.size()==0){
                                    point_vec.push_back(y);
                                    line_vec.push_back(linenum);
                                }else if(before == false){
                                    bool put = true;
                                    for(auto point : point_vec){
                                        if(point==y)put=false;
                                    }
                                    if(put){
                                        point_vec.push_back(y);
                                        line_vec.push_back(linenum);
                                    }
                                }
                                before = true;
                            }else before = false;
                        }
                        bg::clear(edge_line);
                        bg::append(edge_line,polygon.outer().at(0));//始点と終点を結んでる
                        bg::append(edge_line,polygon.outer().at(polygon.outer().size()-1));
                        for(int y=0;y<65;++y){//ここから
                            bool checkline = true;
                            for(auto line : line_vec){
                                if(linenum >= line -1 && linenum <= line+1 )checkline = false;
                            }
                            std::cout << bg::dsv(point_i(line,y)) << bg::dsv(edge_line) << bg::disjoint(point_i(line,y) , edge_line) << std::endl;

                            if(bg::intersects(edge_line, point_i(line,y)) && checkline){//こ↑こ↓の接触判定がガバガバ
                                if(before == false){
                                    bool put = true;
                                    for(auto point : point_vec){
                                        if(point==y)put=false;
                                    }
                                    if(put){
                                        point_vec.push_back(y);
                                        line_vec.push_back(linenum);
                                    }
                                }
                                before = true;
                            }else before = false;
                        }
                        //ここまで
                    }

                    if(point_vec.size() < 2){
                        redo = true;//ここ失敗してたらlineの乱数変えるところからやり直し

                    }
                if(!redo){//redoがfalseなら

                    int linenum = retRnd(point_vec.size() / 2);//分割する線をランダムで指定する
                    begin_line = line_vec[linenum * 2];
                    end_line = line_vec[linenum * 2 + 1];
                    begin_point = point_vec[linenum * 2];
                    end_point = point_vec[linenum * 2 + 1];


                //begin_lineがend_lineより大きい場合の処理を考える
                 if(begin_line > end_line){
                     int tes;//値を一時的においとくだけ
                     tes = begin_line;
                     begin_line = end_line;
                     end_line = tes;
                     tes = begin_point;
                     begin_point = end_point;
                     end_line = tes;
                 }
                 std::cout << bg::dsv(polygon) << std::endl;
                 std::cout << " linenum =" << linenum;
                 std::cout << " pvecsize = " <<point_vec.size();
                 std::cout << " beginline =" << begin_line;
                 std::cout << " endline = "<< end_line;
                 std::cout << " beginpoint = " << begin_point;
                 std::cout << " endpoint = " << end_point;
                 polygon_i polygon1,polygon2;//分割先の複数のpolygonをつくる
                 polygon1.outer().push_back(point_i(line,begin_point));                 //areaが0のピースが存在する→ここの部分で正常に図形がかけてないのでは？   ←多分接触判定の改善で解決する…はず
                 for(int pointnum=begin_line+1;pointnum<end_line+1;pointnum++){
                     polygon1.outer().push_back(polygon.outer().at(pointnum));
                 }
                 polygon1.outer().push_back(point_i(line,end_point));
                 polygon1.outer().push_back(point_i(line,begin_point));

                 polygon2.outer().push_back(point_i(line,begin_point));
                 polygon2.outer().push_back(point_i(line,end_point));
                 for(int pointnum=end_line+1;pointnum<polygon.outer().size()-1;pointnum++){
                     polygon2.outer().push_back(polygon.outer().at(pointnum));
                 }
                 for(int pointnum=0;pointnum<begin_line+1;pointnum++){
                     polygon2.outer().push_back(polygon.outer().at(pointnum));
                 }
                 polygon2.outer().push_back(point_i(line,begin_point));

                 bg::correct(polygon1);
                 bg::correct(polygon2);
                 std::cout << "linex : " << line << "total area : " << bg::area(polygon) << "  p1 area : " << bg::area(polygon1) << "  p2 area : " << bg::area(polygon2) << std::endl;
                 if(bg::area(polygon1) > 300 && bg::area(polygon2) > 300){      //正常に図形が書ければここもよくなるかもしれない
                    print_polygons.erase(print_polygons.begin()+polygon_num);
                    print_polygons.push_back(polygon1);
                    print_polygons.push_back(polygon2);
                    flag = true;
                 }

                 }//redo


            }//x_or_y
            }//flag
        }//area>800
        ++polygon_num;
    }//for rangeでpolygon

    check=false;
    for(auto polygon : print_polygons){
//            if(bg::area(polygon)>800)check=true;//一個でも面積が一定以上のピースがあるならcheckをtrueにしてやり直し    areaが0のピースが生成されてるせいで無限ループを引き起こしてる
    }
}
*/

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
