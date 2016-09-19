#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QByteArray>
#include <QEventLoop>
#include <QUrlQuery>
#include <iostream>
#include <stdexcept>
#include <QSharedPointer>
#include <QMainWindow>
#include <QFile>

#include "solver.h"
#include "polygonio.h"

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
    Ui::MainWindow *ui;
    bool get();
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    bool network_error_flag = false;

private slots:
    void pushGetButton();
    void networkerror(QNetworkReply::NetworkError);
};

#endif // MAINWINDOW_H
