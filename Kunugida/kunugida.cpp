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

#include <iostream>
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
    manager = new QNetworkAccessManager(this);

    board = std::make_shared<NeoAnswerBoard>();
    board->show();
    //    board->setSingleMode(true);
//    board->setSingleMode(true);
}

Kunugida::~Kunugida()
{
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
    // Server
    QObject::connect(&request_mapper,SIGNAL(getAnswer(QString)),this,SLOT(acceptAnswer(QString)));

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

        NeoPolygonIO::exportPolygon(field,"../../procon2017-comp/field.csv");
        emit requestCSV();
        NeoPolygonIO::importField("../../procon2017-comp/field.csv");
    }else if(ui->scanner_button->isChecked()){
        //selected scanner
        logger->info("Selected Scanner DataSource");

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
//        board->setScannedPieces(imrec.getPolygonPosition());

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

//        for(auto& p : field.getElementaryPieces()){
//            NeoPolygonViewer::getInstance().displayPolygon(p.getPolygon(),"hoge",false);
//        }

    }

    for(auto const& p : field.getElementaryPieces()){
        std::cout << boost::geometry::is_valid(p.getPolygon()) << std::endl;
    }

    for(auto const& p : field.getElementaryFrame()){
        std::cout << boost::geometry::is_valid(p.getPolygon()) << std::endl;
    }
    //    TODO: ここまでで各データソースから読み込むようにする

        for(auto p : field.getElementaryPieces()){
            NeoPolygonViewer::getInstance().displayPolygon(p.getPolygon(),"hoge",false);
        }

        int algorithm_number = 0;

    if(ui->test_algorithm_button->isChecked()){
        algorithm_number = 0;
    } else if (ui->beamsearch_button->isChecked()) {
        algorithm_number = 1;
    }

    NeoSolver *solver = new NeoSolver();
    connect(solver,&NeoSolver::throwAnswer,this,&Kunugida::emitAnswer);
    connect(solver, SIGNAL(requestCSV()), this, SLOT(getCSV()));
    connect(this, SIGNAL(requestCSVcomplete()), solver, SLOT(requestCSVcomplete()));
    solver->run(field,algorithm_number);
#endif

    //    QRLibrary lib;
    //    lib.Decoder(true);

    this->finishedProcess();
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

    file.setFileName("../../procon2017-comp/receivedfield.csv");
    if(!file.open(QIODevice::WriteOnly))
        return;
    manager->get(QNetworkRequest(QUrl("http://localhost:8016/get")));
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
