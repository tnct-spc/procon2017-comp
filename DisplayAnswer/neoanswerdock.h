#ifndef NEOANSWERDOCK_H
#define NEOANSWERDOCK_H

#include <QWidget>
#include <QScrollArea>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QSizePolicy>
#include <QFont>
#include "neofield.h"
#include "neosinglepolygondisplay.h"
#include "utilities.h"
#include "neoanswerboard.h"
#include "neopolygonviewer.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv2/opencv.hpp"


namespace Ui {
class NeoAnswerDock;
}

class NeoAnswerDock : public QWidget
{
    Q_OBJECT

public:
    explicit NeoAnswerDock(QWidget *parent = 0);
    ~NeoAnswerDock();
    void addAnswer(procon::NeoField const& field);
    void makePieceList(procon::NeoField field);

private:
    Ui::NeoAnswerDock *ui;
    std::vector<procon::NeoField> fields;
    int c = 1;
private slots:

};

#endif // NEOANSWERDOCK_H
