#include "kunugida.h"
#include "ui_kunugida.h"
#include "qrlibrary.h"
#include "neofield.h"
#include "probmaker.h"
#include "neosolver.h"
#include "neoexpandedpolygon.h"
#include "neopolygonio.h"
#include "polygonio.h"
#include "http/request_mapper.h"
#include "Algorithm/beamsearch.h"
#include "trynextsearch.h"
#include "qrcode.h"

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <cstdlib>

#include <QDebug>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QMessageBox>

Kunugida::Kunugida(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Kunugida)
{
    ui->setupUi(this);
    logger = spdlog::get("Kunugida");

    //    imageRecognitonTest();

    connect(ui->RunButton, &QPushButton::clicked, this, &Kunugida::clickedRunButton);

    connect(this, SIGNAL(requestCSV()), this, SLOT(getCSV()));
    connect(this, SIGNAL(requestpostCSV()), this, SLOT(postCSV()));
    manager = new QNetworkAccessManager(this);

    board = std::make_shared<NeoAnswerBoard>();
    board->show();
    board->showMaximized();

    //Server
    QObject::connect(&request_mapper,SIGNAL(getAnswer(QString)),this,SLOT(acceptAnswer(QString)));
    //    board->setSingleMode(true);
    //    board->setSingleMode(true);

    //スキャナのデバイス名を取得
    char command[256]="sh ../../procon2017-comp/Kunugida/getdevicename.sh";
    if(system(command)==0){
        //うまくデバイス名を取得できたときの処理
        std::cout<<"デバイス名取得できた"<<std::endl;
    }else{
        //デバイス名を取得できなかったときの処理
        std::cout<<"デバイス名取得できない"<<std::endl;
    }
}

Kunugida::~Kunugida()
{
    system("echo > devicename.txt");
    delete ui;
}

void Kunugida::run()
{
    logger->info("Run Button Clicked");

    procon::NeoField field;

    auto polygoniToExpanded = [](std::vector<polygon_i> pieces_,std::vector<int> id_list){


        std::vector<procon::ExpandedPolygon> expanded_pieces;
        int id = 0;
        for(auto &piece_i : pieces_){
            polygon_t piece_t;
            for(auto &point : piece_i.outer()){
                int double_point_x = point.x();
                int double_point_y = point.y();
                piece_t.outer().push_back(point_t(double_point_x,double_point_y));
            }
            bg::correct(piece_t);

            procon::ExpandedPolygon ex_poly(id_list.at(id));
            ex_poly.resetPolygonForce(piece_t);
            expanded_pieces.push_back(ex_poly);
            id++;
        }

        return expanded_pieces;

    };

    if(ui->probmaker_button->isChecked()){
        //selected probmaker
        logger->info("Selected ProbMaker DataSource");

        ProbMaker *PbMaker = new ProbMaker();

        //もしProbMakerの結果を表示したければ下をコメントアウト
        //std::string test_text = "test";
        //board->setText(test_text);
        PbMaker->show();
        std::vector<polygon_i> pieces_ = PbMaker->getPieces();
        polygon_i frame_ = PbMaker->getFrame();

//#define SHOW_ANGLE
#ifdef SHOW_ANGLE
        std::cout << "angles ->" << std::endl;
        std::vector<double> angles;
        for(polygon_i piece : pieces_) {
            procon::NeoExpandedPolygon neoPiece;
            neoPiece.resetPolygonForce(piece);
            for(double angle : neoPiece.getSideAngle()) {
                angle = (angle / M_PI) * 180;
                angles.push_back(angle);
            }
        }

        std::sort(angles.begin(), angles.end());

        for(double angle : angles) {
            std::cout << angle << ", ";
        }
        std::cout << std::endl << std::endl;
    }
#else

        std::vector<procon::NeoExpandedPolygon> pieces;
        procon::NeoExpandedPolygon frame;

        int id = 0;
        for(auto& piece : pieces_){
            procon::NeoExpandedPolygon buf(id);
            buf.resetPolygonForce(piece);
            pieces.push_back(buf);
            ++id;
            //break;

        }
        frame.resetPolygonForce(frame_);
        std::vector<procon::NeoExpandedPolygon> vec_frame;
        vec_frame.push_back(frame);
        field.setElementaryFrame(vec_frame);
        field.setElementaryPieces(pieces);

        int scanned_dummy_piece_id = 0;
        std::vector<procon::ExpandedPolygon> scanned_dummy_piece;
        for(auto polygon : pieces){
            polygon_t poly_buf;
            for(auto p : polygon.getPolygon().outer()){
                poly_buf.outer().push_back(point_t(p.x(),p.y()));
            }
            procon::ExpandedPolygon ex_buf(scanned_dummy_piece_id);
            ex_buf.resetPolygonForce(poly_buf);
            scanned_dummy_piece.push_back(ex_buf);
            ++scanned_dummy_piece_id;
        }

        board->setScannedPieces(scanned_dummy_piece);

        NeoPolygonIO::exportPolygon(field,"../../procon2017-comp/field.csv");
        emit requestCSV();

    }else if(ui->scanner_button->isChecked()){
        //selected scanner
        logger->info("Selected Scanner DataSource");

        cv::Mat first_scan = scanImage();
        cv::Mat second_scan;
        if(!first_scan.empty()){
            cv::imshow("いっこめ",first_scan);
            QMessageBox message_box;
            message_box.setText("スキャン実行");
            QPushButton *button0 = message_box.addButton(tr("cansel"),QMessageBox::ActionRole);
            QPushButton *button1 = message_box.addButton(tr("start"),QMessageBox::ActionRole);
            message_box.exec();
            if(message_box.clickedButton() == button0){
                //キャンセル時
            }else if(message_box.clickedButton() == button1){
                //二回目実行時
                second_scan = scanImage();
                cv::imshow("にこめ",second_scan);
            }
        }
    }else if(ui->image_data_button->isChecked()){
        //selected image
        logger->info("Selected ImageData DataSource");

        cv::Mat frame = cv::imread("../../procon2017-comp/sample/frame.png", 1);
        cv::Mat pieces = cv::imread("../../procon2017-comp/sample/pices.png", 1);

        ImageRecognition imrec;
        field = imrec.run(frame, pieces);

        //        imageRecognitonTest();
//        ImageRecognition imrec;
//        field = imrec.run(frame, pieces);
        board->setScannedPieces(imrec.getPolygonPosition());

        //        imageRecognitonTest();
    }else if(ui->csv_button->isChecked()){
        //CSV date
        std::string path = QFileDialog::getOpenFileName(this,"SELECT CSV","./../../procon2017-comp/DebugFieldCsv",tr("Text files(*.csv)")).toStdString();
        field = NeoPolygonIO::importField(path);



        std::vector<polygon_i> poly_pieces;
        std::vector<int> id_list;
        for(const auto poly : field.getElementaryPieces()){
            id_list.push_back(poly.getId());
            polygon_i i_poly = poly.getPolygon();
            poly_pieces.push_back(i_poly);
        }
        std::vector<procon::ExpandedPolygon> ex_poly = polygoniToExpanded(poly_pieces , id_list);
        board->setScannedPieces(ex_poly);

    }else if(ui->sample_data_use_button->isChecked()){
        //read sample
        field = NeoPolygonIO::importField("../../procon2017-comp/sample/comp-sample.csv");
        //dummy
        std::vector<procon::ExpandedPolygon> scanned_poly;
        for(const auto& p : field.getElementaryPieces()){
            polygon_t poly_buf;

            for(const auto& p : p.getPolygon().outer()){
                poly_buf.outer().push_back(point_t(p.x(),p.y()));
            }

            procon::ExpandedPolygon exp_buf(p.getId());
            exp_buf.resetPolygonForce(poly_buf);
            scanned_poly.push_back(exp_buf);
        }

        board->setScannedPieces(scanned_poly);


    }else if(ui->chinochan_button->isChecked()){
        bool is_hint = false;
        bool is_multi = false;
        int how_qr = 1;
        if(ui->is_hint->isChecked()) is_hint = true;
        if(ui->is_multi->isChecked()){
            is_multi = true;
            how_qr = ui->how_qr->value();
        }
        QRCode qrcode;
        qrcode.Decoder(true, is_hint, is_multi, how_qr);
        field = NeoPolygonIO::importField("../../procon2017-comp/CSV/fromQRcode.csv");
        int scanned_dummy_piece_id = 0;

        std::vector<procon::ExpandedPolygon> scanned_dummy_piece;
        for(auto polygon : field.getElementaryPieces()){
            polygon_t poly_buf;
            for(auto p : polygon.getPolygon().outer()){
                poly_buf.outer().push_back(point_t(p.x(),p.y()));
            }
            procon::ExpandedPolygon ex_buf(scanned_dummy_piece_id);
            ex_buf.resetPolygonForce(poly_buf);
            scanned_dummy_piece.push_back(ex_buf);
            ++scanned_dummy_piece_id;
        }

        board->setScannedPieces(scanned_dummy_piece);
    }

    if(ui->ServerModeCheckbox->isChecked()){
        std::string PROBLEM_SAVE_PATH = "../../procon2017-comp/CSV/problem.csv";
        std::cout << "Save problem in : " << PROBLEM_SAVE_PATH << std::endl;
        NeoPolygonIO::exportPolygon(field, PROBLEM_SAVE_PATH);

    }else{
    //    TODO: ここまでで各データソースから読み込むようにする

    int algorithm_number = 0;

    if(ui->test_algorithm_button->isChecked()){
        algorithm_number = 0;
    } else if (ui->beamsearch_button->isChecked()) {
        algorithm_number = 1;
    }

    NeoSolver *solver = new NeoSolver(ui->beamwidth->value());
    connect(solver,&NeoSolver::throwAnswer,this,&Kunugida::emitAnswer);
    connect(solver, SIGNAL(requestCSV()), this, SLOT(getCSV()));
    connect(this, SIGNAL(requestCSVcomplete()), solver, SLOT(requestCSVcomplete()));
    solver->run(field,algorithm_number);

#endif
//    QRLibrary lib;
//    lib.Decoder(true);
    this->finishedProcess();
    }
}

void Kunugida::clickedRunButton()
{
    if(!this->is_running){
        logger->info("start solving process");
        this->startProcess();
        this->run();
    }else{
        //warning
        logger->warn("solving process is already running");
    }
}

void Kunugida::emitAnswer(procon::NeoField field)
{
   logger->info("emitted answer");
   std::cout << field.getPieces().size() << std::endl;
   this->board->setField(field);
   this->board->update();
}

void Kunugida::finishedProcess()
{
    this->is_running = false;
}


void Kunugida::startProcess()
{
    this->is_running = true;
}

void Kunugida::imageRecognitonTest()
{
    std::cout << "Hello ImageRecogniton Test" << std::endl;

    cv::Mat nocframe = cv::imread("./../../procon2017-comp/sample/sample_frame_3.JPG", 1);
    cv::Mat nocpieces = cv::imread("/home/spc/ダウンロード/real_piece5", 1);

    ImageRecognition imrec;
    procon::NeoField PDATA = imrec.run(nocframe, nocpieces);
}

void Kunugida::getCSV()
{
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    file.setFileName("../../procon2017-comp/CSV/receivedfield.csv");
    if(!file.open(QIODevice::WriteOnly))
        return;
    manager->get(QNetworkRequest(QUrl("http://localhost:8016/get")));
}

void Kunugida::postCSV()
{
    file.setFileName("../../procon2017-comp/CSV/post.csv");
    if(!file.open(QIODevice::ReadOnly))
        return;
    manager->post(QNetworkRequest(QUrl("http://localhost:8016/answer")), &file);
}

void Kunugida::replyFinished(QNetworkReply *reply)
{
    QString str;
    str = "[Network]";
    if(reply->error() == QNetworkReply::NoError){
        str += tr("download success.");
        file.write(reply->readAll());
        file.close();
    }else{
        str = reply->errorString();
        str += tr("      download failed.");
        file.close();
    }
    qDebug() << str;
    emit requestCSVcomplete();
}

cv::Mat Kunugida::scanImage()
{
    time_t t = time(NULL);
    char fileName[256];
    strcpy(fileName , ctime(&t));
    for(char &c : fileName) if(c == ' ' || c == '\n') c = '_';
    strcat(fileName , ".png");
    std::cout<<fileName<<std::endl;

    char command[256]="sh ../../procon2017-comp/Kunugida/getimage.sh ";
    strcat(command , fileName);

    cv::Mat mat;
    if(system(command)==0){
        //うまくスキャンしたときの処理
        std::cout<<"スキャンできた"<<std::endl;
        mat = cv::imread(fileName);
    }else{
        //残念ながらスキャンできなかったときの処理
        std::cout<<"スキャンできない"<<std::endl;
    }
    return mat;
}
