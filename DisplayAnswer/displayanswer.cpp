#include "displayanswer.h"


DisplayAnswer::DisplayAnswer()
{
    board.showMaximized();
}

void DisplayAnswer::setField(Field &field)
{
    this->field = field;
    board.setField(this->field);
}
