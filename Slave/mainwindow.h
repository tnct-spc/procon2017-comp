#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "solver.h"
#include "polygonio.h"
#include "answerboard.h"
#include "QNetworkAccessManager"
#include "QNetworkReply"
#include "QNetworkRequest"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    std::shared_ptr<AnswerBoard> board;
    Ui::MainWindow *ui;
    bool get();
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    bool network_error_flag = false;

private slots:
    void pushGetButton();
    void networkerror(QNetworkReply::NetworkError);
};

#endif // MAINWINDOW_H
