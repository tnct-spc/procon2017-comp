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

    void addAnswer(const procon::NeoField &field , const std::string &text);
    void setSelectAnswerMode(bool mode);

private:
    Ui::NeoAnswerDock *ui;
    std::vector<procon::NeoField> fields;
    std::vector<procon::NeoField> pieces;

    bool select_answer_mode = false;

private slots:
    void submitedField(procon::NeoField field);

signals:
    void selectedAnswer(procon::NeoField field);

};

#endif // NEOANSWERDOCK_H
