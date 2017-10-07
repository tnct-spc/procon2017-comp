#ifndef THRESHOLDGUI_H
#define THRESHOLDGUI_H

#include <QWidget>

namespace Ui {
class THresholdGUI;
}

class THresholdGUI : public QWidget
{
    Q_OBJECT

public:
    explicit THresholdGUI(QWidget *parent = 0);
    ~THresholdGUI();

private:
    Ui::THresholdGUI *ui;
};

#endif // THRESHOLDGUI_H
