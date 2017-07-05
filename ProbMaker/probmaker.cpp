#include "probmaker.h"
#include "ui_probmaker.h"
#include "neosinglepolygondisplay.h"

#include <boost/geometry/geometry.hpp>
#include <boost/math/tools/fraction.hpp>
#include <boost/math/common_factor_rt.hpp>

#include <numeric>
#include <random>

#include <QPainter>

ProbMaker::ProbMaker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProbMaker)
{
    ui->setupUi(this);
    this->run();
}

ProbMaker::~ProbMaker()
{
    delete ui;
}

void ProbMaker::run()
{
    //polygonのvectorから一番面積が大きいやつのindexを持ってくるやつ
    auto greatest_area = [](std::vector<polygon_i> polygons)->unsigned int{

        int current_biggest_polygon_number = 0;
        double current_biggest_polygon_area = 0;
        int current_polygon_number = 0;

        for(auto polygon : polygons){
            double current_area = boost::geometry::area(polygon);
            if(current_biggest_polygon_area < current_area){
                current_biggest_polygon_area = current_area;
                current_biggest_polygon_number = current_polygon_number;
            }
            ++current_polygon_number;
        }
        return current_biggest_polygon_number;
    };

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
        
        int start_dot_num = start_dot_random(rnd);
        int end_dot_num = end_dot_random(rnd);

        //new points vector
        std::vector<point_i> new_points1;
        std::vector<point_i> new_points2;

        int a = 0;
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

            for (int index = 0; index < polygon.outer().size() - 1; ++index) {
                if(!check_same_point(polygon.outer().at(index),polygon.outer().at(index+1))){
                    deleted_duplicated_point_polygon.outer().push_back(polygon.outer().at(index));
                }
            }
            
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

    auto connect_polygon = [](){
        //polygon同士の結合
        std::cout << "connect polygon" << std::endl;
    };
    
    //本番の101*65
    polygon_i base_polygon;
    base_polygon.outer().push_back(point_i(0,0));
    base_polygon.outer().push_back(point_i(0,65));
    base_polygon.outer().push_back(point_i(101,65));
    base_polygon.outer().push_back(point_i(101,0));
    base_polygon.outer().push_back(point_i(0,0));

    auto polygons = devide_polygon(base_polygon);
    this->print_polygons.push_back(polygons.first);
    this->print_polygons.push_back(polygons.second);
}

void ProbMaker::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QPen(QColor("#000000")));
    constexpr int grid_size = 17;
    constexpr int max_col = 101;
    constexpr int max_row = 61;
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

    auto draw_polygon = [&painter,&grid_size](polygon_i polygon){
        std::vector<QPoint> print_points;
        for(auto point : polygon.outer()){
            print_points.push_back(QPoint(point.x()*grid_size,point.y()*grid_size));
        }
        painter.drawPolygon(&print_points.front(),print_points.size());
    };

    for(auto polygon : this->print_polygons){
        draw_polygon(polygon);
    }
}


