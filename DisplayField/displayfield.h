#ifndef DISPLAYFIELD_H
#define DISPLAYFIELD_H

#include <QMainWindow>
#include <QPushButton>
#include "neofield.h"
#include "neopolygonio.h"
#include "neoanswerboard.h"

namespace Ui {
class DisplayField;
}

class DisplayField : public QMainWindow
{
    Q_OBJECT

public:
    explicit DisplayField(QWidget *parent = 0);
    ~DisplayField();

private:
    procon::NeoField field;
    Ui::DisplayField *ui;
    std::shared_ptr<NeoAnswerBoard> board;


private slots:
    void runAnswerBoard();
    void selectFieldData();
};

#endif // DISPLAYFIELD_H
