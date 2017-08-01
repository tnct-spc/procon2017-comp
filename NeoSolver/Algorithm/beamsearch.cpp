#include "beamsearch.h"

#define DEBUG_MODE

void connect_polygon(polygon_i& frame, polygon_i& connecting_polygon,Connect connection){



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
}

void BeamSearch::run(procon::NeoField field){

}
