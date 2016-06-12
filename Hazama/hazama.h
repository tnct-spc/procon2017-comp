#ifndef HAZAMA_H
#define HAZAMA_H

#include <QMainWindow>
#include <opencv2/core/core.hpp>

namespace Ui {
class Hazama;
}

class Hazama : public QMainWindow
{
    Q_OBJECT

public:
    explicit Hazama(QWidget *parent = 0);
    ~Hazama();
    bool WebcameraEnable = false;

private:
    Ui::Hazama *ui;
    void init();
    void run();
    cv::Mat capture();
private slots:
    void clickedRunButton();
    void checkedWebCameraEnable();
};

#endif // HAZAMA_H
