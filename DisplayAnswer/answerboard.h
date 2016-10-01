#ifndef ANSWERBOARD_H
#define ANSWERBOARD_H

#include "field.h"

class AnswerBoard;

namespace Ui {
class AnswerBoard;
}

class AnswerBoard : public QWidget
{
    Q_OBJECT

public:
    explicit AnswerBoard(QWidget *parent = 0);
    ~AnswerBoard();
    bool SINGLE_MODE = false;
    void setField(const procon::Field& field);
    static void setRawPicture(const cv::Mat& raw_pieces_pic,const std::vector<cv::Point>& pieces_pos);
    static void setRandomColors(const std::vector<cv::Vec3b>& random_colors);

signals:
    void clicked();

private slots:
    void printBigWindow();

private:
    Ui::AnswerBoard *ui;
    std::unique_ptr<procon::Field> field;
    static std::unique_ptr<QImage> pieces_pic;
    static std::unique_ptr<std::vector<cv::Point>> pieces_pos;
    static std::unique_ptr<std::vector<cv::Vec3b>> random_colors;
    bool is_set_field = false;
    static bool is_set_rawpic;
    std::vector<int> putid_list;
    int putid_left = -1;
    int putid_right = -1;

    const int top_margin    = 100;
    const int bottom_margin = 10;
    const int left_margin   = 100;
    const int right_margin  = 10;
    const double frame_size = 35;

    procon::Field print_field;
    AnswerBoard* ans_board;

    enum Space
    {
        LEFT    = 0,
        RIGHT   = 1,
        OVERALL = 2
    };

    QPointF getPosition(QPointF point_percent, Space space);

    double getScale();

protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent*);
};

#endif // ANSWERBOARD_H
