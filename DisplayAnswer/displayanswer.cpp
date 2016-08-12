#include "displayanswer.h"

DisplayAnswer::DisplayAnswer()
{
    board = std::make_shared<AnswerBoard>();
    board->showMaximized();
}

void DisplayAnswer::setField(procon::Field &field)
{
    is_set_field = true;
    board->setField(field);
}

void DisplayAnswer::setRawPicture(cv::Mat raw_pieces_pic, std::vector<cv::Point> raw_pieces_pos)
{
    is_set_rawpic = true;
    board->setRawPicture(raw_pieces_pic, raw_pieces_pos);
}
