#ifndef DISPLAYANSWER_H
#define DISPLAYANSWER_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "answerboard.h"
#include "field.h"
#include "displayanswer_global.h"

class DISPLAYANSWERSHARED_EXPORT DisplayAnswer
{

public:
    DisplayAnswer();
    void setField(procon::Field& field);
    void setRawPicture(cv::Mat raw_pieces_pic, std::vector<cv::Point> raw_pieces_pos);
private:
    std::shared_ptr<AnswerBoard> board;
    bool is_set_field = false;
    bool is_set_rawpic = false;
};

#endif // DISPLAYANSWER_H
