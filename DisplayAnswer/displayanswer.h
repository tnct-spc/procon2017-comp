#ifndef DISPLAYANSWER_H
#define DISPLAYANSWER_H

#include "answerboard.h"
#include "field.h"
#include "displayanswer_global.h"

class DISPLAYANSWERSHARED_EXPORT DisplayAnswer
{

public:
    DisplayAnswer();
    std::shared_ptr<AnswerBoard> board;
    void setField(procon::Field& field);
private:
    procon::Field field;
};

#endif // DISPLAYANSWER_H
