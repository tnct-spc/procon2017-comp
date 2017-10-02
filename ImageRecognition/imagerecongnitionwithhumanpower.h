#ifndef IMAGERECONGNITIONWITHHUMANPOWER_H
#define IMAGERECONGNITIONWITHHUMANPOWER_H

#include <QWidget>
#include "precompile.h"
#include "polygonviewer.h"

namespace Ui {
class imagerecongnitionwithhumanpower;
}

class imagerecongnitionwithhumanpower : public QWidget
{
    Q_OBJECT

public:
    explicit imagerecongnitionwithhumanpower(QWidget *parent = 0);
    ~imagerecongnitionwithhumanpower();

private:
    Ui::imagerecongnitionwithhumanpower *ui;

private slots:

protected:

signals:

};

#endif // IMAGERECONGNITIONWITHHUMANPOWER_H
