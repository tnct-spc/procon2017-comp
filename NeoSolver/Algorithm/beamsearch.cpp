#include "beamsearch.h"

#define DEBUG_MODE

BeamSearch::BeamSearch()
{

}

void BeamSearch::init()
{
#ifdef DEBUG_MODE
    std::cout << "DEBUG MODE" << std::endl;
#else
    std::cout << "EFFICIENT MODE" << std::endl;
#endif
}

void BeamSearch::run(procon::NeoField field){

}
