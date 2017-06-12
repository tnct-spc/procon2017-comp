#include "probmaker.h"
#include "ui_probmaker.h"

#include <boost/geometry/geometry.hpp>

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
    //Make Problem

    std::random_device random;
    std::uniform_int_distribution<> col_rand(0,101 - 1);
    std::uniform_int_distribution<> row_rand(0,61 - 1);

    for (int var = 0; var < 100; ++var) {
        std::cout << col_rand(random) << std::endl;
        std::cout << row_rand(random) << std::endl;
    }

    polygon_i sample_polygon;
    sample_polygon.outer().push_back(point_i(0,0));
    sample_polygon.outer().push_back(point_i(0,1));
    sample_polygon.outer().push_back(point_i(1,1));
    sample_polygon.outer().push_back(point_i(0,0));
    polygon_i sample_polygon2;
    sample_polygon2.outer().push_back(point_i(0,0));
    sample_polygon2.outer().push_back(point_i(0,2));
    sample_polygon2.outer().push_back(point_i(2,2));
    sample_polygon2.outer().push_back(point_i(0,0));

    std::vector<polygon_i> sample_polygon_vector;
    sample_polygon_vector.push_back(sample_polygon);
    sample_polygon_vector.push_back(sample_polygon2);


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
    auto devide_polygon = [](polygon_i polygon){
        std::random_device rnd;
        std::uniform_int_distribution<> random(0,polygon.outer().size() - 2);

        std::cout << "これが辺選択のランダムさをおおおおおおぉおおおおお" << random(rnd) << std::endl;
        
        int start_sides = random(rnd);
        int end_sizes = 0; 
        do {
            [](){}();
            
        } while (end_sizes = 0);

    };

    devide_polygon(sample_polygon);

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
}


