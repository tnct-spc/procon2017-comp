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

#include "Algorithm/algorithmwrapper.h"
#include "Algorithm/hillclibming.h"
#include "Algorithm/pooralgorithm.h"
#include "Algorithm/simplealgorithm.h"
#include "Utils/fit.h"
#include "Utils/polygonconnector.h"
#include "answerboard.h"
#include "expandedpolygon.h"
#include "field.h"
#include "imagerecognition.h"
#include "imagerecognition_global.h"
#include "polygon.h"
#include "polygonio.h"
#include "polygonviewer.h"
#include "singlepolygondisplay.h"
#include "solver.h"
#include "utilities.h"

class TesterWraper
{
public:
    TesterWraper();
    bool run(){return false;}
    void print(std::string filename, int line, std::string string = "ok"){
        std::cout<<"["<<filename<<"]"<<"["<<std::to_string(line)<<"] "<<string<<std::endl;
    }
};

#endif // TESTERWRAPER_H
