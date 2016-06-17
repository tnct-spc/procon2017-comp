#ifndef DISPLAYANSWER_H
#define DISPLAYANSWER_H

#include "answerboard.h"
#include "field.h"
#include "displayanswer_global.h"

class DISPLAYANSWERSHARED_EXPORT DisplayAnswer
{

public:
    DisplayAnswer();
    AnswerBoard board;
    void setField(Field& field);
private:
    Field field;
};

#endif // DISPLAYANSWER_H
