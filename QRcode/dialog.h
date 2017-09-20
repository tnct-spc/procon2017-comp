#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <qrlibrary.h>


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
  Q_OBJECT

public:
  explicit Dialog(QWidget *parent = 0);
  ~Dialog();

private:
  Ui::Dialog *ui;
private slots:
    void Quit();
};

#endif // DIALOG_H
