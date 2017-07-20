#ifndef QRLIBRUARY_H
#define QRLIBRUARY_H

#include <iostream>
#include <string>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <zbar.h>

class QRLibruary
{
public:
    QRLibruary();
    std::string Decoder(bool s);
};

#endif // QRLIBRUARY_H
