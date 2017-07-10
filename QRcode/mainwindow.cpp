#include "mainwindow.h"
#include "ui_mainwindow.h"

std::ofstream result("result.txt");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Buttons
    connect(ui->exitButton, &QPushButton::clicked,
            this, &MainWindow::Quit);
    connect(ui->okButton, &QPushButton::clicked,
            this, &MainWindow::takePicture);
    //Menu bar
    connect(ui->action_Quit, &QAction::triggered,
            this, &MainWindow::Quit);
}

using namespace cv;

void MainWindow::takePicture()
{
    QRLibruary lib;
    decodedCode = lib.Decoder(isAuto());
    std::cout << "----------------------Result----------------------\n" << decodedCode << std::endl;
    result << decodedCode << std::endl;
    Quit(0);

}

bool MainWindow::isAuto()
{
    bool s;
    if(ui->libruaryEnabler->checkState() == Qt::Checked)
        s = true;
    else
        s = false;
    return s;
}

void MainWindow::Quit(int code)
{
    std::cout << "Program finished in code " << code << std::endl;
    exit(code);
}

MainWindow::~MainWindow()
{
    delete ui;
}
