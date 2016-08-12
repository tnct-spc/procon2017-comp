#ifndef ANSWERBOARD_H
#define ANSWERBOARD_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QWidget>
#include "field.h"

namespace Ui {
class AnswerBoard;
}

class AnswerBoard : public QWidget
{
    Q_OBJECT

public:
    explicit AnswerBoard(QWidget *parent = 0);
    ~AnswerBoard();
    void setField(procon::Field& field);
    void setRawPicture(cv::Mat raw_pieces_pic, std::vector<cv::Point> pieces_pos);

private:
    Ui::AnswerBoard *ui;
    procon::Field field;
    QImage pieces_pic;
    std::vector<cv::Point> pieces_pos;
    bool is_set_field = false;
    bool is_set_rawpic = false;

protected:
    void paintEvent(QPaintEvent *);
};

#endif // ANSWERBOARD_H
