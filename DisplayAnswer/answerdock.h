#ifndef ANSWERDOCK_H
#define ANSWERDOCK_H

#include <QWidget>

#include "field.h"

namespace Ui {
class AnswerDock;
}

class AnswerDock : public QWidget
{
    Q_OBJECT

public:
    explicit AnswerDock(QWidget *parent = 0);
    ~AnswerDock();
    void addAnswer(procon::Field const& new_field);


private:
    Ui::AnswerDock *ui;
    std::vector<procon::Field> fields;
};

#endif // ANSWERDOCK_H
