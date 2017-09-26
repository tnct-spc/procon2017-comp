
#ifndef QRLIBRUARY_H
#define QRLIBRUARY_H

#include <iostream>
#include <string>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <zbar.h>

#include "qrtranslatetopolygon.h"

class QRLibrary
{
public:
    QRLibrary();
    std::pair<std::vector<polygon_i>, std::vector<polygon_i>> Decoder(bool s);
//    void debug(std::string code);

private:
    bool is_multi = false;
    bool read = false;

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // QRLIBRUARY_H
