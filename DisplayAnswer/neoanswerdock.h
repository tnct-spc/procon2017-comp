#ifndef NEOANSWERDOCK_H
#define NEOANSWERDOCK_H

#include <QWidget>
#include <QScrollArea>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include "field.h"
#include "utilities.h"
#include "answerboard.h"


namespace Ui {
class NeoAnswerDock;
}

class NeoAnswerDock : public QWidget
{
    Q_OBJECT

public:
    explicit NeoAnswerDock(QWidget *parent = 0);
    ~NeoAnswerDock();
    void addAnswer(procon::Field const& field);

private:
    Ui::NeoAnswerDock *ui;
    std::vector<procon::Field> fields;
private slots:
    void clickedAnswer(int id);
signals:
    void selectField(procon::Field field);
};

#endif // NEOANSWERDOCK_H
