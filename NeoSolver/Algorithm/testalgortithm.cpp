#include "testalgortithm.h"

TestAlgortithm::TestAlgortithm()
{

}

void TestAlgortithm::init()
{

}

void TestAlgortithm::run(procon::NeoField field)
{
    std::cout << "called test algrotihm" << std::endl;
    field.setFrame(field.getElementaryFrame());
    submitAnswer(field);
}
