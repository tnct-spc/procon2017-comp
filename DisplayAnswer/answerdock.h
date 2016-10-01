#ifndef ANSWERDOCK_H
#define ANSWERDOCK_H

#include <QWidget>

#include "field.h"
#include "answerboard.h"

namespace Ui {
class AnswerDock;
}

class AnswerDock : public QWidget
{
    Q_OBJECT

public:
    explicit AnswerDock(QWidget *parent = 0);
    ~AnswerDock();
    static void setupDefaultRawPicData(const cv::Mat& raw_pieces_pic_, const std::vector<cv::Point>& raw_pieces_pos_, const std::vector<cv::Vec3b>& raw_random_colors_);
    void addAnswer(procon::Field const& field);

    static cv::Mat raw_pieces_pic;
    static std::vector<cv::Point> raw_pieces_pos;
    static std::vector<cv::Vec3b> raw_random_colors;


private:
    Ui::AnswerDock *ui;
};

#endif // ANSWERDOCK_H
