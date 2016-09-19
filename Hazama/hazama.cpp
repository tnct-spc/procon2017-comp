
#include "hazama.h"
#include "ui_hazama.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QFileDialog>
#include <QMessageBox>

#include "field.h"
#include "imagerecognition.h"
#include "polygonio.h"
#include "solver.h"

Hazama::Hazama(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Hazama)
{
    ui->setupUi(this);
    connect(ui->RunButton, &QPushButton::clicked, this, &Hazama::clickedRunButton);
    init();
}

Hazama::~Hazama()
{
    delete ui;
}

void Hazama::init()
{
    board = std::make_shared<AnswerBoard>();
    board->showMaximized();

    //selectWebCamera(); <- ha?

    QObject::connect(&request_mapper,SIGNAL(getAnswer(QString)),this,SLOT(acceptAnswer(QString)));
}

void Hazama::acceptAnswer(QString file_path)
{
    std::cout<<"accept!!!!!!!!!!!!!!!!!"<<std::endl;
    //get POST puzzle answer data
    procon::Field field = procon::PolygonIO::importAnswer(file_path.toStdString(),PDATA);

    if(first_answer_flag){
        first_answer_flag = false;
        best_answer = field;
    }else{
        if(field.getPiecesSize() > best_answer.getPiecesSize()){
            best_answer = field;
        }
    }

    //Display answer
    board->setField(best_answer);
}

void Hazama::makeCalibrationData(std::string savefile_path,unsigned int numberOfImages)
{
    //This is Library... Black box
    //Reference : http://yuki-sato.com/wordpress/2016/04/15/opencv-%E3%82%AB%E3%83%A1%E3%83%A9%E3%81%AE%E6%AD%AA%E3%81%BF%E3%82%92%E3%81%AA%E3%81%8A%E3%81%99%E3%82%AD%E3%83%A3%E3%83%AA%E3%83%96%E3%83%AC%E3%83%BC%E3%82%B7%E3%83%A7%E3%83%B3-c/

    using namespace cv;
    using namespace std;

    namedWindow("debugwindow", CV_WINDOW_AUTOSIZE);

    int horizonalCrossCount = 10;
    int verticalCrossCount = 7;

    // calibrate points
    vector<vector<Point3f>> object_points;
    vector<vector<Point2f>> image_points;
    vector<Point3f> obj;
    for(int j=0;j< horizonalCrossCount * verticalCrossCount;j++) {
        obj.push_back(Point3f(j/horizonalCrossCount, j%horizonalCrossCount, 0.0f));
    }

    for(unsigned int i=1; i<numberOfImages; i++) {

        std::cout << "now:" << i << std::endl;

        char filename[128];
        sprintf(filename, "../../procon2016-comp/calibration/%d.jpg", i);
        Mat frame = imread(filename);
        Mat gray;

        flip(frame, frame, -1);
        cvtColor(frame, gray, CV_BGR2GRAY);

        // 10-7 チェスを探す
        Size chessboardPatterns(horizonalCrossCount, verticalCrossCount);
        vector<Point2f> centers;
        bool found = findChessboardCorners(gray, chessboardPatterns, centers, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);
        if (found) {
            // 見つけたpointを高精度化する
            cornerSubPix(gray, centers, Size(11,11), Size(-1,-1), TermCriteria (TermCriteria::EPS+TermCriteria::COUNT, 30, 0.1));
            object_points.push_back(obj);
            image_points.push_back(centers);

            // draw
            drawChessboardCorners(gray, chessboardPatterns, Mat(centers), true);
            imshow("debugWindow", gray);

        } else {
            cout << "not found" << endl;
        }
    }

    // calibrate
    vector<Mat> rvecs, tvecs;
    Mat mtx, dist;
    calibrateCamera(object_points, image_points, Size(1920,1080), mtx, dist, rvecs, tvecs);

    // save
    FileStorage fs(savefile_path, FileStorage::WRITE);
    fs << "mtx" << mtx;
    fs << "dist" << dist;
    fs.release();
}

void Hazama::run()
{
    //When you want to calibrate webcamera,please comment out this line!!!
    //makeCalibrationData("./../../procon2016-comp/calibration/calibration.yml",24);


    std::cout << "Run" << std::endl;

    std::string flame_path = "./../../procon2016-comp/sample/1_flame.png";
    std::string pieces_path = "./../../procon2016-comp/sample/1_pieces.png";
    std::string path = "./../../procon2016-comp/sample/data.csv";

    /*Get puzzle data*/
    if(ui->useWebCamera->isChecked() || ui->useImageData->isChecked()){
        cv::Mat flame;
        cv::Mat pieces;
        //get Image
        if(ui->useWebCamera->isChecked()){

            //slectWebCamra device number
            device_number = selectWebCamera();

            //camera open
            capture(device_number);

            cv::namedWindow("capture",cv::WINDOW_AUTOSIZE);
            cv::Mat pressentertextwindow = cv::Mat::zeros(100,700,CV_8UC3);
            cv::putText(pressentertextwindow, "Please Press Enter",cv::Point(0,100),cv::FONT_HERSHEY_SCRIPT_SIMPLEX,2.4,cv::Scalar(255,255,255),2,CV_AA);
            cv::Mat pressentertextsecondwindow = cv::Mat::zeros(100,700,CV_8UC3);
            cv::putText(pressentertextsecondwindow,"Please Press Enter",cv::Point(0,100),cv::FONT_HERSHEY_SCRIPT_SIMPLEX,2.4,cv::Scalar(0,255,255),2,CV_AA);
            cv::imshow("capture",pressentertextwindow);
            while(cv::waitKey(0)==13);
            flame = capture(device_number);
            cv::imshow("capture",pressentertextsecondwindow);
            while(cv::waitKey(0)==13);
            pieces = capture(device_number);
            cv::imwrite("flame.png",flame,std::vector<int>(CV_IMWRITE_PNG_COMPRESSION,0));
            cv::imwrite("pieces.png",pieces,std::vector<int>(CV_IMWRITE_PNG_COMPRESSION,0));
            cv::destroyWindow("capture");

        } else {

            //環境によっては動かない
            //std::string flame_path = QFileDialog::getOpenFileName(this,"input flame picture","./../../procon2016-comp/picture/").toStdString();
            //std::string pieces_path = QFileDialog::getOpenFileName(this,"input pieces picture","./../../procon2016-comp/picture/").toStdString();

            flame = cv::imread(flame_path, 1);
            pieces = cv::imread(pieces_path, 1);
        }

        /*Image Recognition*/
        ImageRecognition imrec;
        PDATA = imrec.run(flame, pieces);

        //display recognized image
        board->setRawPicture(imrec.getRawPiecesPic(), imrec.getRawPiecesPos());
        board->setRandomColors(imrec.getRawRandomColors());
    } else if (ui->useFileData->isChecked()) {
        //環境によっては動かない
        //std::string path = QFileDialog::getOpenFileName(this).toStdString();
        PDATA = procon::PolygonIO::importPolygon(path);
    } else {
        return;
    }

    if(ui->server_mode_radio->isChecked()){
        /*Save Puzzle*/
        std::string PROBLEM_SAVE_PATH = QCoreApplication::applicationDirPath().toStdString()+"/docroot/problem.csv";
        std::cout<<PROBLEM_SAVE_PATH<<std::endl;
        procon::PolygonIO::exportPolygon(PDATA, PROBLEM_SAVE_PATH);
    }else{
        //Solve puzzle
        Solver solver;
        int algorithm_number = -1;
        if(ui->algo0->isChecked()){
            algorithm_number = 0;
        }else if(ui->algo1->isChecked()){
            algorithm_number = 1;
        }else if(ui->algo2->isChecked()){
            algorithm_number = 2;
        }else if(ui->algo3->isChecked()){
            algorithm_number = 3;
        }else{
            throw "poa";
            //poa
        }
        procon::Field field = solver.run(PDATA, algorithm_number);

        //Display answer
        board->setField(field);
    }

    std::cout<<"finish"<<std::endl;
}

cv::Mat Hazama::capture(int deviceNumber)
{
    const std::string calibration_data_file_path = "./../../procon2016-comp/calibration/calibration.yml";
    static bool init_calibration_flag = false;

    static cv::Mat mtx,dist;

    //init
    if(!init_calibration_flag){
        init_calibration_flag = true;

        cv::FileStorage fs(calibration_data_file_path, cv::FileStorage::READ);
        fs["mtx"] >> mtx;
        fs["dist"] >> dist;
        fs.release();
    }


    cv::VideoCapture cap(deviceNumber);//デバイスのオープン
    //cap.open(0);//こっちでも良い．

    if(!cap.isOpened()){
        std::cerr << "Couldn't open camera!" << std::endl;
    }

    //setting
    cap.set(cv::CAP_PROP_FRAME_WIDTH,1920);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT,1080);
    cap.set(cv::CAP_PROP_BRIGHTNESS,0.4);
    cap.set(cv::CAP_PROP_FOCUS,0.3);
    cap.set(cv::CAP_PROP_SATURATION,80);
    cap.set(cv::CAP_PROP_BACKLIGHT,20);

    cv::Mat src;

    //too slow...
    for(int i=0;i<30;i++){
        cap >> src;
    }

    //calibrate
    cv::Mat calibration_src;
    cv::undistort(src, calibration_src, mtx, dist);

    return calibration_src;
}

void Hazama::clickedRunButton()
{
    run();
}

int Hazama::selectWebCamera()
{
    QMessageBox MsgBox;
    QPushButton *button0 = MsgBox.addButton(tr("WebCamera0"),QMessageBox::ActionRole);
    QPushButton *button1 = MsgBox.addButton(tr("WebCamera1"),QMessageBox::ActionRole);

    MsgBox.setText("Please select deveice number");

    MsgBox.exec();

    if (MsgBox.clickedButton() == button0) {
        std::cout << "0" << std::endl;
        return 0;
    } else if(MsgBox.clickedButton() == button1) {
        std::cout << "1" << std::endl;
        return 1;
    }
    MsgBox.deleteLater();
    return 0;
}
