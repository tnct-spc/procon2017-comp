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
    explicit AnswerBoard(int id = -1, bool is_single_mode = false, QWidget *parent = 0);
    ~AnswerBoard();

    void setField(const procon::Field& field);
    static void setRawPicture(const cv::Mat& raw_pieces_pic,const std::vector<cv::Point>& pieces_pos);
    static void setRandomColors(const std::vector<cv::Vec3b>& random_colors);

signals:
    void clicked(QMouseEvent*event);
    void clicked_with_id(int id);

private slots:
    void handleClickAction(QMouseEvent*event);

private:
    Ui::AnswerBoard *ui;
    const int top_margin    = 100;
    const int bottom_margin = 10;
    const int left_margin   = 100;
    const int right_margin  = 10;
    const double frame_size = 35;

    // Only field mode
    const bool SINGLE_MODE = false;

    // Field(left)
    bool is_set_field = false;
    std::unique_ptr<procon::Field> field;

    // RawPic(right)
    static bool is_set_rawpic;
    static std::unique_ptr<QImage> pieces_pic;
    static std::unique_ptr<std::vector<cv::Point>> pieces_pos;
    static std::unique_ptr<std::vector<cv::Vec3b>> random_colors;

    // Support put piece arrow
    std::vector<int> putid_list;
    int putid_left = -1;
    int putid_right = -1;
    const int id = -1;

    // Big board
    AnswerBoard* ans_board;

    enum Space
    {
        LEFT    = 0,
        RIGHT   = 1,
        OVERALL = 2
    };

    // Get position to board (min-0, max-1)
    QPointF getPosition(QPointF point_percent, Space space);

    double getScale();

protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent*event);
};

#endif // ANSWERBOARD_H
