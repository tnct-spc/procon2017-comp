#ifndef TESTERWRAPER_H
#define TESTERWRAPER_H

#include <QApplication>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMainWindow>
#include <QPainter>
#include <QPen>
#include <QWidget>
#include <algorithm>
#include <array>
#include <boost/assign/list_of.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/disjoint.hpp>
#include <boost/geometry/algorithms/distance.hpp>
#include <boost/geometry/algorithms/transform.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <cmath>
#include <exception>
#include <functional>
#include <iostream>
#include <array>
#include <memory>
#include <numeric>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/photo.hpp>
#include <opencv2/photo/photo.hpp>
#include <random>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <vector>

#define print(S) std::cout<<"["<<__FILE__<<"]"<<"["<<std::to_string(__LINE__)<<"] "<<S<<std::endl;

class TesterWraper
{
public:
    TesterWraper();
    virtual bool run(){print("マクロあるで");return true;}
};

#endif // TESTERWRAPER_H
