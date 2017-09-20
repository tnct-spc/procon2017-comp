#ifndef QRLIBRUARY_H
#define QRLIBRUARY_H

#include <QWidget>
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
//    std::pair<std::vector<polygon_i>,polygon_i> Decoder(bool s);
    std::string Decoder(bool s);
private:
};

#endif // QRLIBRUARY_H
