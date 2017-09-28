#ifndef NEOSLAVER_H
#define NEOSLAVER_H

#include <QWidget>

namespace Ui {
class neoslaver;
}

class neoslaver : public QWidget
{
    Q_OBJECT

public:
    explicit neoslaver(QWidget *parent = 0);
    ~neoslaver();

private:
    Ui::neoslaver *ui;
};

#endif // NEOSLAVER_H
