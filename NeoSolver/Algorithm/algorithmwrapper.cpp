#include "algorithmwrapper.h"

AlgorithmWrapper::AlgorithmWrapper()
{

}


AlgorithmWrapper::~AlgorithmWrapper()
{

}

void AlgorithmWrapper::init(){

}

void AlgorithmWrapper::run(procon::NeoField field)
{
    std::cout << "algorithmwarpper called" << std::endl;
    return;
}

void AlgorithmWrapper::submitAnswer(procon::NeoField field)
{
    std::cout << "algorithm warpper throwanswr called" << std::endl;
    emit throwAnswer(field);
    // Wait 1msec
    QEventLoop loop;
    QTimer::singleShot(1, &loop, SLOT(quit()));
    loop.exec();
}

std::pair<int, std::vector<procon::NeoExpandedPolygon>> setPieceToFrame(std::vector<procon::NeoExpandedPolygon> frame_vector, procon::NeoExpandedPolygon piece, std::vector<std::tuple<int, int, int, int>> tuples)
{
    for(std::tuple<int, int, int, int> tuple : tuples) {
        int frame_vector_index = std::get<0>(tuple), freme_index = std::get<1>(tuple), polygon_index = std::get<2>(tuple), score = std::get<3>(tuple);
        procon::NeoExpandedPolygon frame = frame_vector.at(frame_vector_index);
        auto rotate_and_move = [](polygon_i polygon_before, polygon_i polygon_after, int before_index, int after_index)
        {
            std::vector<point_i> points_before = polygon_before.outer(), points_after = polygon_after.outer();
            point_i point_before1 = points_before.at(before_index), point_before2 = points_before.at(before_index + 1);
            point_i point_after1 = points_after.at(after_index), point_after2 = points_after.at(after_index + 1);
            std::complex<double> complex_before(static_cast<double> (point_before2.x() - point_before1.x()), static_cast<double> (point_before2.y() - point_before1.y()));
            std::complex<double> complex_after(static_cast<double> (point_after2.x() - point_after1.x()), static_cast<double> (point_after2.y() - point_after1.y()));
            std::complex<double> complex = complex_after / complex_before;

            for() {

            }
        };
    }
}
