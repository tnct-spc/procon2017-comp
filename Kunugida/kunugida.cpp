#include "kunugida.h"
#include "ui_kunugida.h"
#include "qrlibrary.h"
#include "neofield.h"
#include "probmaker.h"
#include "neosolver.h"
#include "neoexpandedpolygon.h"
#include "neopolygonio.h"

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

    board = std::make_shared<NeoAnswerBoard>();
    tcp = std::make_shared<TcpMain>();
    board->show();
    tcp->show();
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

        std::vector<procon::NeoExpandedPolygon> pieces;
        procon::NeoExpandedPolygon frame;

        int id = 1;
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
    }

    for(auto const& p : field.getPieces()){
        std::cout << boost::geometry::is_valid(p.getPolygon()) << std::endl;
    }

    for(auto const& p : field.getFrame()){
        std::cout << boost::geometry::is_valid(p.getPolygon()) << std::endl;
    }
    //    TODO: ここまでで各データソースから読み込むようにする

    int algorithm_number = 0;

    if(ui->test_algorithm_button->isChecked()){
        algorithm_number = 0;
    } else if (ui->beamsearch_button->isChecked()) {
        algorithm_number = 1;
    }

//   Show piece list
    list->makePieceList(field);

    NeoSolver *solver = new NeoSolver();
    connect(solver,&NeoSolver::throwAnswer,this,&Kunugida::emitAnswer);
    solver->run(field,algorithm_number);


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
   this->board->setField(NeoPolygonIO::importField("../../procon2017-comp/field.csv"));
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

    cv::Mat nocframe = cv::imread("/home/spc/ダウンロード/real_frame3.png", 1);
    cv::Mat nocpieces = cv::imread("/home/spc/ダウンロード/real_piece3.png", 1);

    ImageRecognition imrec;
    procon::NeoField PDATA = imrec.run(nocframe, nocpieces);
}
