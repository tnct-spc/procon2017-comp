#ifndef NEOANSWERDOCK_H
#define NEOANSWERDOCK_H

#include <QWidget>
#include <QScrollArea>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include "neofield.h"
#include "utilities.h"
#include "neoanswerboard.h"


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
    void addAnswer(const procon::NeoField &field , const std::string &text);
private:
    Ui::NeoAnswerDock *ui;
    std::vector<procon::NeoField> fields;
};

#endif // NEOANSWERDOCK_H
