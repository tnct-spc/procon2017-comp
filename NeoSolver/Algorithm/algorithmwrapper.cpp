#include "algorithmwrapper.h"

AlgorithmWrapper::AlgorithmWrapper()
{

}


AlgorithmWrapper::~AlgorithmWrapper()
{

}

void AlgorithmWrapper::init(){

}


void AlgorithmWrapper::submitAnswer(procon::NeoField field){
    emit throwAnswer(field);
}
