#include "polygonconnector.h"

PolygonConnector::PolygonConnector()
{

}

std::pair<procon::NeoExpandedPolygon, procon::NeoExpandedPolygon> PolygonConnector::connect(procon::NeoExpandedPolygon field, procon::NeoExpandedPolygon piece, Connect connecter)
{
    int freme_index1,frame_index2, polygon_index1,polygon_index2;
        procon::NeoExpandedPolygon frame = frame_vector.at(frame_vector_index);
        auto rotate_and_move = [](procon::NeoExpandedPolygon expolygon_before, procon::NeoExpandedPolygon expolygon_after, int before_index, int after_index)
        {
            polygon_i polygon_before = expolygon_before.getPolygon(), polygon_after = expolygon_after.getPolygon();
            std::vector<point_i> points_before = polygon_before.outer(), points_after = polygon_after.outer();
            point_i point_before1 = points_before.at(before_index), point_before2 = points_before.at(before_index + 1);
            point_i point_after1 = points_after.at(after_index), point_after2 = points_after.at(after_index + 1);
            std::complex<double> complex_before(static_cast<double> (point_before2.x() - point_before1.x()), static_cast<double> (point_before2.y() - point_before1.y()));
            std::complex<double> complex_after(static_cast<double> (point_after2.x() - point_after1.x()), static_cast<double> (point_after2.y() - point_after1.y()));
            std::complex<double> complex = complex_after / complex_before;
        };
}
