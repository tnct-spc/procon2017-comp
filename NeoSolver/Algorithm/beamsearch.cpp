#include "beamsearch.h"
#include "expandedpolygon.h"
#include "neopolygonviewer.h"
#include "polygonviewer.h"

#define DEBUG_MODE

namespace boost {
    namespace geometry {
        namespace math {
                inline bool equals(double const& a,double const& b){
                    std::cout << "yaeehehhh" << std::endl;
                    return a == b;
                }
            }
        }
    }

BeamSearch::BeamSearch()
{
    logger = spdlog::get("beamsearch");
}

void BeamSearch::init()
{
#ifdef DEBUG_MODE
    logger->info("debug mode");
#else
    logger->info("efficient mode");
#endif
    this->processor_num = std::thread::hardware_concurrency();
}

void BeamSearch::run(procon::NeoField field)
{
    std::cout << "called beamsearch" << std::endl;
    boost::geometry::equals(point_t(2.0,2.0),point_t(2.0,2.0));
    boost::geometry::math::equals(1.0,2.0);

}
