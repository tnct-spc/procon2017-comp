#include "displayanswer.h"


DisplayAnswer::DisplayAnswer()
{
}

void DisplayAnswer::setField(procon::Field &field)
{
    board = std::make_shared<AnswerBoard>();
    board->showMaximized();
    this->field = field;
    board->setField(this->field);
}
