﻿#include "neoslaver.h"
#include "ui_neoslaver.h"
#include "neosolver.h"

NeoSlaver::NeoSlaver(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NeoSlaver)
{
    ui->setupUi(this);
    board = std::make_shared<NeoAnswerBoard>();
    board->showMaximized();
    connect(ui->run, &QPushButton::clicked, this, &NeoSlaver::pushGetButton);
    board->setSingleMode(true);
}

NeoSlaver::~NeoSlaver()
{
    delete ui;
    delete manager;
}

bool NeoSlaver::get()
{
    if(ui->boss->isChecked()){
        SERVER_URL = "http://localhost:8016/get";
        SERVER_POST_URL = "http://localhost:8016/answer";
        SAVE_PROBLEM_PATH = "../../procon2017-comp/CSV/problem-slave.csv";
        SAVE_ANSWER_PATH = "../../procon2017-comp/CSV/answer-slave.csv";
    }

    QEventLoop eventloop;

    std::cout<<"challange get"<<std::endl;

    //get
    connect(manager,SIGNAL(finished(QNetworkReply*)),&eventloop,SLOT(quit()));
    QUrl requrl = SERVER_URL;
    QNetworkReply *getreply = manager->get(QNetworkRequest(requrl));
    getreply->ignoreSslErrors();

    connect(getreply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(networkerror(QNetworkReply::NetworkError)));
    eventloop.exec();

    if(network_error_flag) return false;
    std::string raw_data = getreply->readAll().constData();

    std::cout<<"get ok"<<std::endl;

    //save src
    std::ofstream outputfile(SAVE_PROBLEM_PATH);
    outputfile << raw_data;
    outputfile.close();

    //io
    procon::NeoField PDATA;
    PDATA = NeoPolygonIO::importField(SAVE_PROBLEM_PATH);

    //solve puzzle
    solver = new NeoSolver(ui->beamwidth->value(), ui->answer_progress->isChecked());
    connect(solver,&NeoSolver::throwAnswer,this,&NeoSlaver::emitAnswer);
    int algorithm_number = 0;
    if(ui->algo1->isChecked()){
        algorithm_number = 0;
    }else if(ui->algo2->isChecked()){
        algorithm_number = 1;
    }else{
        throw "poa";
        //poa nari
    }

    procon::NeoField test = PDATA;

    solver->run(PDATA, algorithm_number);
    return true;
}

bool NeoSlaver::emitAnswer(procon::NeoField field)
{
    QEventLoop eventloop;

    //Display answer
    board->setField(field);

    //io
    NeoPolygonIO::exportPolygon(field,SAVE_ANSWER_PATH);

    std::cout<<"challenge send"<<std::endl;

    //send
    QFile answer_file;
    answer_file.setFileName(QString::fromStdString(SAVE_ANSWER_PATH));
    answer_file.open(QIODevice::ReadOnly);
    connect(manager,SIGNAL(finished(QNetworkReply*)),&eventloop,SLOT(quit()));
    QNetworkReply *postreply = manager->post(QNetworkRequest(QUrl(SERVER_POST_URL)), &answer_file);
    connect(postreply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(networkerror(QNetworkReply::NetworkError)));
    eventloop.exec();

    //std::cout<<"sending :"<<postData.toString(QUrl::FullyEncoded).toUtf8().toStdString()<<std::endl;
    answer_file.close();

    if(network_error_flag) return false;

    std::cout<<"send ok"<<std::endl;

    ui->state->setText(QString::fromStdString(std::string(postreply->readAll().constData())));
    this->board->setField(field);
    this->board->update();

    return true;
}

bool NeoSlaver::debugEmitAnswer(procon::NeoField field)
{
    QEventLoop eventloop;

    //Display answer
    board->setField(field);

    //io
    NeoPolygonIO::exportPolygon(field,SAVE_ANSWER_PATH);

    std::cout<<"challenge send"<<std::endl;

    //send
    QFile answer_file;
    answer_file.setFileName(QString::fromStdString(SAVE_ANSWER_PATH));
    answer_file.open(QIODevice::ReadOnly);
    connect(manager,SIGNAL(finished(QNetworkReply*)),&eventloop,SLOT(quit()));
    QNetworkReply *postreply = manager->post(QNetworkRequest(QUrl(SERVER_POST_URL)), &answer_file);
    connect(postreply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(networkerror(QNetworkReply::NetworkError)));
    eventloop.exec();

    //std::cout<<"sending :"<<postData.toString(QUrl::FullyEncoded).toUtf8().toStdString()<<std::endl;
    answer_file.close();

    if(network_error_flag) return false;

    std::cout<<"send ok"<<std::endl;

    ui->state->setText(QString::fromStdString(std::string(postreply->readAll().constData())));
    this->board->setField(field);
    this->board->update();

    return true;
}

void NeoSlaver::pushGetButton()
{
    setIPaddress(ui->ip_address->text());
    if(!get()){
        ui->getLabel->setText("Faild Run");
    }else{
        ui->getLabel->setText("Success Run!");
    }
}

void NeoSlaver::networkerror(QNetworkReply::NetworkError e)
{
    /*
     * エラーコードについては
     * Qtリファレンスの
     * QNetworkReply Class -> enum QNetworkReply::NetworkError
     * に乗ってます
     */
    int code = e;
    std::cout << "network error! error code  : " << code  << std::endl;
    network_error_flag = true;
    //net_error_num = e;
}

void NeoSlaver::setIPaddress(QString ip_address)
{
    SERVER_URL = "http://" + ip_address + "/get";
    SERVER_POST_URL = "http://" + ip_address + "/answer";
}
