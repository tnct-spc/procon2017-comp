#ifndef KUNUGIDA_H
#define KUNUGIDA_H

#include <QMainWindow>

namespace Ui {
class Kunugida;
}

class Kunugida : public QMainWindow
{
    Q_OBJECT

public:
    explicit Kunugida(QWidget *parent = 0);
    ~Kunugida();

private:
    Ui::Kunugida *ui;
};

#endif // KUNUGIDA_H
