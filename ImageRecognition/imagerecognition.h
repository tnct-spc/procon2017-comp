#ifndef IMAGERECOGNITION_H
#define IMAGERECOGNITION_H

#include <QMainWindow>

namespace Ui {
class ImageRecognition;
}

class ImageRecognition : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageRecognition(QWidget *parent = 0);
    ~ImageRecognition();
    void run();

private:
    Ui::ImageRecognition *ui;
};

#endif // IMAGERECOGNITION_H
