#ifndef NEOANSWERDOCK_H
#define NEOANSWERDOCK_H

#include <QWidget>
#include <QScrollArea>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include "neofield.h"
#include "neosinglepolygondisplay.h"
#include "utilities.h"
#include "neoanswerboard.h"
#include "neopolygonviewer.h"


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
    void makePieceList(procon::NeoField const& field);

private:
    Ui::NeoAnswerDock *ui;
    std::vector<procon::NeoField> fields;
    procon::NeoField pieces;
    std::vector<std::unique_ptr<NeoSinglePolygonDisplay>> temp;
};

#endif // NEOANSWERDOCK_H
