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
    delaunay_triangulation();
    GA();
    makeHint();
}

ProbMaker::~ProbMaker()
{
    delete ui;
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
int ProbMaker::retRnd(int num){
    std::random_device rand;
    std::mt19937 mt(rand());
    std::uniform_int_distribution<int> rnd(0,num-1);//randomを返すだけ
    return rnd(mt);
}
void ProbMaker::makeHint(){
    double mostinstanceHintsNumber = print_polygons.size() * 0.9;
    double miminstanceHintsNumber = print_polygons.size() * 0.8;
    std::vector<polygon_i> piece = print_polygons;
    int mostHintsNumber = mostinstanceHintsNumber;
    int mimHintsNumber = miminstanceHintsNumber;
    int HintsNumber1,HintsNumber2,HintsNumber3,HintsNumber4;
    do{
    HintsNumber1 = retRnd(100) + 1;
    HintsNumber2 = retRnd(100) + 1;
    HintsNumber3 = retRnd(100) + 1;
    HintsNumber4 = retRnd(100) + 1;
    }while((HintsNumber1 + HintsNumber2 + HintsNumber3 + HintsNumber4) < mimHintsNumber || (HintsNumber1 + HintsNumber2 + HintsNumber3 + HintsNumber4) > mostHintsNumber);
    std::string example = std::to_string(HintsNumber1);
    disposition1 += example;
    //ヒント一段階目
    for(int i = 0; i < HintsNumber1;i++){
        int rnd = retRnd(piece.size());
       polygon_i instancepiece = piece.at(rnd);
       piece.erase(piece.begin() + rnd);
       int counter = 0;
       disposition1 += ":";
       disposition1 += std::to_string(instancepiece.outer().size() - 1);
       for(auto point_a : instancepiece.outer()){
           int point_x = point_a.x();
           int point_y = point_a.y();
           if(counter != 0){
                   disposition1 += " ";
                   disposition1 += std::to_string(point_x);
                   disposition1 += " ";
                   disposition1 += std::to_string(point_y);
               }
           counter++;
           }\
                }

    //ヒント二段階目
    example = std::to_string(HintsNumber2);
    disposition2 += example;
    for(int a = 0; a < HintsNumber2;a++){
        int rnd = retRnd(piece.size());
       polygon_i instancepiece = piece.at(rnd);
       piece.erase(piece.begin() + rnd);
       int counter = 0;
       disposition2 += ":";
       disposition2 += std::to_string(instancepiece.outer().size() -1);
       for(auto point_a : instancepiece.outer()){
           int point_x = point_a.x();
           int point_y = point_a.y();
           if(counter != 0){
                   disposition2 += " ";
                   disposition2 += std::to_string(point_x);
                   disposition2 += " ";
                   disposition2 += std::to_string(point_y);
               }
           counter++;
           }
                }

    //ヒント三段階目
    example = std::to_string(HintsNumber3);
    disposition3 += example;
    for(int b = 0; b < HintsNumber3;b++){
        int rnd = retRnd(piece.size());
       polygon_i instancepiece = piece.at(rnd);
       piece.erase(piece.begin() + rnd);
       int counter = 0;
       disposition3 += ":";
       disposition3 += std::to_string(instancepiece.outer().size() - 1);
       for(auto point_a : instancepiece.outer()){
           int point_x = point_a.x();
           int point_y = point_a.y();
           if(counter != 0){
                   disposition3 += " ";
                   disposition3 += std::to_string(point_x);
                   disposition3 += " ";
                   disposition3 += std::to_string(point_y);
           }
           counter++;
                }
            }
    //ヒント四段階目
    example = std::to_string(HintsNumber4);
    disposition4 += example;
    for(int c = 0; c < HintsNumber4;c++){
        int rnd = retRnd(piece.size());
       polygon_i instancepiece = piece.at(rnd);
       piece.erase(piece.begin() + rnd);
       int counter = 0;
       disposition4 += ":";
       disposition4 += std::to_string(instancepiece.outer().size() - 1);
       for(auto point_a : instancepiece.outer()){
           int point_x = point_a.x();
           int point_y = point_a.y();
           if(counter != 0){
                   disposition4 += " ";
                   disposition4 += std::to_string(point_x);
                   disposition4 += " ";
                   disposition4 += std::to_string(point_y);
               }
                   counter++;
           }
                }

    std::cout<<print_polygons.size()<<" "<<mimHintsNumber<<std::endl;
    std::cout<<HintsNumber1<<" "<<HintsNumber2<<" "<<HintsNumber3<<" "<<HintsNumber4<<" "<< HintsNumber1 + HintsNumber2 + HintsNumber3 + HintsNumber4 <<std::endl;
    std::cout <<disposition4<<std::endl;
    //ここから形状情報
    /*
    std::vector<polygon_i> shapepiece;
    shapeHints += std::to_string(print_polygons.size());
    shapeHints += ':';
    polygon_i polygon2;
    bool result;
    for(polygon_i shape : print_polygons){
        polygon2.clear();
        int random_x = retRnd(10000);
        int random_y = retRnd(10000);
        do{
        for(point_i shapepoint : shape.outer()){
            int point_x = shapepoint.x();
            int point_y = shapepoint.y();
            point_x = point_x + random_x;
            point_y = point_y + random_y;
            polygon2.outer().push_back(point_i(point_x,point_y));
            }
        for(polygon_i poly : shapepiece){
            result = bg::disjoint(poly,polygon2);
            if(result)break;
            }
        }while(result);
        shapepiece.push_back(polygon2);
    }
    for(polygon_i polygonZ : shapepiece){
        int counter = 0;
        for(point_i pointZ : polygonZ){
            int shape_x = pointZ.x();
            int shape_y = pointZ.y();
            if(counter != 0){
                if(counter == 1){
                    shapeHints += ":";
                    shapeHints += std::to_string(shape_x);
                    shapeHints += " ";
                    shapeHints += std::to_string(shape_y);
                }
                if(counter != 1){
                    shapeHints += " ";
                    shapeHints += std::to_string(shape_x);
                    shapeHints += " ";
                    shapeHints += std::to_string(shape_y);
                }
            }
            counter++;
        }
    }
    */
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
