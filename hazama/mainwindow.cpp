#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <opencv2/core/core.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{

}

void MainWindow::run()
{
    std::cout << "Run" << std::endl;

    /*Take a picture*/
    capture();

    /*Image Recognition*/


    /*Solve puzzle*/

    /*Display answer*/

}

void MainWindow::clickedRunButton()
{
    run();
}

void MainWindow::capture()
{

}
