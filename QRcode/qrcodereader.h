#ifndef QRCODEREADER_H
#define QRCODEREADER_H

#include <QWidget>

namespace Ui {
class QRcodereader;
}

class QRcodereader : public QWidget
{
    Q_OBJECT

public:
    explicit QRcodereader(QWidget *parent = 0);
    ~QRcodereader();

private:
    Ui::QRcodereader *ui;
};

#endif // QRCODEREADER_H
