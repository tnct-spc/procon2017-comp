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
    void addAnswer(procon::Field const& field);

private:
    Ui::AnswerDock *ui;
    std::vector<procon::Field> fields;
private slots:
    void clickedAnswer(int id);
signals:
    void selectField(procon::Field field);

};

#endif // ANSWERDOCK_H
