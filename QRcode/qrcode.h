//新世代
#ifndef QRCODE_H
#define QRCODE_H

#include <QWidget>
#include <QCheckBox>
#include <QSpinBox>
#include <string>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <zbar.h>
#include "qrtranslatetopolygon.h"

namespace Ui {
class QRCode;
}

class QRCode : public QWidget
{
    Q_OBJECT

public:
    explicit QRCode(QWidget *parent = 0);
    ~QRCode();
    std::pair<std::vector<polygon_i>,std::vector<polygon_i>> Decoder(bool s, bool is_hint, bool hint_multi, int how_qr);

private:
    Ui::QRCode *ui;
    bool read = false;
    bool is_multi = false;
};

#endif // QRCODE_H
