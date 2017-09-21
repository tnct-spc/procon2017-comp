#ifndef QREADER_H
#define QREADER_H

#include <QWidget>
#include <string>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <zbar.h>

#include "qrtranslatetopolygon.h"

namespace Ui {
class QReader;
}

class QReader : public QWidget
{
  Q_OBJECT

public:
  explicit QReader(QWidget *parent = 0);
  std::string Decoder();
  ~QReader();

private:
  Ui::QReader *ui;
  bool c = 1;
  int code_count = 0;

private slots:
  void Quit();
  void stop();
};

#endif // QREADER_H
