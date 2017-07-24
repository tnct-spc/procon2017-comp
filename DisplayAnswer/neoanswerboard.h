#ifndef NEOANSWERBOARD_H
#define NEOANSWERBOARD_H

#include <QWidget>
#include <vector>

#include "field.h"
#include "neoexpandedpolygon.h"

namespace Ui {
class NeoAnswerBoard;
}

class NeoAnswerBoard : public QWidget
{
    Q_OBJECT

public:
    explicit NeoAnswerBoard(QWidget *parent = 0);
    ~NeoAnswerBoard();

private:
    Ui::NeoAnswerBoard *ui;
    std::vector<polygon_i> piecepolygon;
    polygon_i framepolygon;
    QPointF getPosition(point_i point);
    int grid_size;
    int top_buttom_margin;
    int left_right_margin;
    int top_margin;

    //Field field;

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // NEOANSWERBOARD_H
