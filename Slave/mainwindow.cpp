#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete manager;
}

bool MainWindow::get()
{
    std::string SAVE_PROBLEM_PATH = QCoreApplication::applicationDirPath().toStdString()+"/docroot/problem.csv";
    std::string SAVE_ANSWER_PATH = QCoreApplication::applicationDirPath().toStdString()+"/docroot/answer.csv";
    QString SERVER_URL = "http://127.0.0.1:8016/get";
    QString SERVER_POST_URL = "http://127.0.0.1:8016/answer";
    QEventLoop eventloop;

    std::cout<<"challange get"<<std::endl;
    //

    //get
    connect(manager,SIGNAL(finished(QNetworkReply*)),&eventloop,SLOT(quit()));
    QUrl requrl = SERVER_URL;
    QNetworkReply *getreply = manager->get(QNetworkRequest(requrl));
    getreply->ignoreSslErrors();

    std::cout<<"endd"<<std::endl;
    connect(getreply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(networkerror(QNetworkReply::NetworkError)));
    eventloop.exec();

    std::cout<<"end"<<std::endl;
    if(network_error_flag) return false;
    std::string raw_data = getreply->readAll().constData();

    std::cout<<"finish get"<<std::endl;

    //save src
    std::ofstream outputfile(SAVE_PROBLEM_PATH);
    outputfile << raw_data;
    outputfile.close();

    //io
    procon::Field PDATA = procon::PolygonIO::importPolygon(SAVE_PROBLEM_PATH);

    //solve puzzle
    Solver solver;
    procon::Field field = solver.run(PDATA);

    //io
    procon::PolygonIO::exportAnswer(field,SAVE_ANSWER_PATH);

    std::cout<<"challenge send"<<std::endl;

    //send
    QFile answer_file(QString::fromStdString(SAVE_ANSWER_PATH));
    answer_file.open(QIODevice::ReadOnly);
    QUrlQuery postData;
    postData.addQueryItem("answer",answer_file.readAll().toStdString().c_str());
    postData.addQueryItem("id",ui->ID->text());
    QNetworkRequest req(SERVER_POST_URL);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    connect(manager,SIGNAL(finished(QNetworkReply*)),&eventloop,SLOT(quit()));
    QNetworkReply *postreply = manager->post(req,postData.toString(QUrl::FullyEncoded).toUtf8());
    connect(postreply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(networkerror(QNetworkReply::NetworkError)));
    eventloop.exec();

    std::cout<<"sending :"<<postData.toString(QUrl::FullyEncoded).toUtf8().toStdString()<<std::endl;
    answer_file.close();

    if(network_error_flag) return false;

    std::cout<<"finish send"<<std::endl;

    ui->state->setText(QString::fromStdString(std::string(postreply->readAll().constData())));

    return true;
}

void MainWindow::pushGetButton()
{
    if(!get()){
        ui->getLabel->setText("Faild Run");
    }else{
        ui->getLabel->setText("Success Run!");
    }
}

void MainWindow::networkerror(QNetworkReply::NetworkError e)
{
    /*
     * エラーコードについては
     * Qtリファレンスの
     * QNetworkReply Class -> enum QNetworkReply::NetworkError
     * に乗ってます
     */
    int code = e;
    std::cout << "network error code " << code  << std::endl;
    network_error_flag = true;
    //net_error_num = e;
}
