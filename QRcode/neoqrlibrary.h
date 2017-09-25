#ifndef NEOQRLIBRARY_H
#define NEOQRLIBRARY_H

#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <zbar.h>
#include <vector>
#include <string>
#include <iostream>
#include "qrtranslatetopolygon.h"

class NeoQRLibrary
{
public:
    NeoQRLibrary();
    std::pair<std::vector<polygon_i>, polygon_i> Decoder(bool s);

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // NEOQRLIBRARY_H
