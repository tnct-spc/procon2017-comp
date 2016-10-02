#include "hazama.h"
#include "ui_hazama.h"

#include "polygonviewer.h"

#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>

Hazama::Hazama(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Hazama)
{
    ui->setupUi(this);
    connect(ui->RunButton, &QPushButton::clicked, this, &Hazama::clickedRunButton);

    //configGUI
    connect(ui->LoadConfig, &QPushButton::clicked, this, &Hazama::loadInIFile);
    connect(ui->WriteConfig, &QPushButton::clicked, this, &Hazama::makeInIFile);

    thresholdGUIinit();

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

    //Display answer
    emitAnswer(field);
}

void Hazama::emitAnswer(procon::Field field)
{
    if(first_answer_flag){
        first_answer_flag = false;
        best_answer = field;
    }else{
        if(field.getFrame().getJointedPieces().size() > best_answer.getFrame().getJointedPieces().size()){
            best_answer = field;
        }
    }

    //Display answer
    board->setField(best_answer);
    //PolygonViewer::getInstance().pushPolygon(field.getFrame(),std::string("Answer Frame"));
    //PolygonViewer::getInstance().pushPolygon(field.getFrame().getJointedPieces().at(0),std::string("Answer Piece No.0"));
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

    std::string frame_path = "./../../procon2016-comp/sample/mirrorless_nframe.JPG";

    //disable threshold UI
    disableThresholdUI();

    std::string pieces_path = "./../../procon2016-comp/sample/mirrorless_npieces.JPG";
    std::string path = "./../../procon2016-comp/sample/data.csv";

    /*Get puzzle data*/
    if(ui->useWebCamera->isChecked() || ui->useImageData->isChecked()){
        cv::Mat raw_frame;
        cv::Mat raw_pieces;
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
            raw_frame = capture(device_number);
            cv::imshow("capture",pressentertextsecondwindow);
            while(cv::waitKey(0)==13);
            raw_pieces = capture(device_number);
            cv::imwrite("frame.png",raw_frame,std::vector<int>(CV_IMWRITE_PNG_COMPRESSION,0));
            cv::imwrite("pieces.png",raw_pieces,std::vector<int>(CV_IMWRITE_PNG_COMPRESSION,0));
            cv::destroyWindow("capture");

        } else {

            //環境によっては動かない
            //std::string frame_path = QFileDialog::getOpenFileName(this,"input frame picture","./../../procon2016-comp/picture/").toStdString();
            //std::string pieces_path = QFileDialog::getOpenFileName(this,"input pieces picture","./../../procon2016-comp/picture/").toStdString();

            raw_frame = cv::imread(frame_path, 1);
            raw_pieces = cv::imread(pieces_path, 1);
        }

        /*Image Recognition*/
        PDATA = imrec.run(raw_frame, raw_pieces);

        //display recognized image
        AnswerBoard::setRawPicture(imrec.getRawPiecesPic(), imrec.getRawPiecesPos());
        AnswerBoard::setRandomColors(imrec.getRawRandomColors());
    } else if (ui->useFileData->isChecked()) {
        //環境によっては動かない
        //std::string path = QFileDialog::getOpenFileName(this).toStdString();
        PDATA = procon::PolygonIO::importPolygon(path);
    } else {
        return;
    }

    // Hide HAZAMA
    this->showMinimized();

    if(ui->server_mode_radio->isChecked()){
        /*Save Puzzle*/
        std::string PROBLEM_SAVE_PATH = QCoreApplication::applicationDirPath().toStdString()+"/docroot/problem.csv";
        std::cout<<PROBLEM_SAVE_PATH<<std::endl;
        procon::PolygonIO::exportPolygon(PDATA, PROBLEM_SAVE_PATH);
    }else{
        //Solve puzzle
        solver = new Solver();
        connect(solver,&Solver::throwAnswer,this,&Hazama::emitAnswer);
        int algorithm_number = -1;
        if(ui->algo0->isChecked()){
            algorithm_number = 0;
        }else if(ui->algo1->isChecked()){
            algorithm_number = 1;
        }else if(ui->algo2->isChecked()){
            algorithm_number = 2;
        }else if(ui->algo3->isChecked()){
            algorithm_number = 3;
        }else if(ui->algo4->isChecked()){
            algorithm_number = 4;
        }else if(ui->algo5->isChecked()){
            algorithm_number = 5;
        }else{
            throw "poa";
            //poa
        }
        solver->run(PDATA, algorithm_number);
    }



    //enable thresholdUI
    enableThresholdUI();

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
    cap.set(cv::CAP_PROP_BRIGHTNESS,0.3);
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

void Hazama::makeInIFile()
{
    boost::property_tree::ptree config;

    //put
    config.put("test.test",13);

    //write
    boost::property_tree::write_ini("./../../procon2016-comp/config/threshold.ini",config);

    /* threhsold.ini write example
     *
     * [test]
     * test=13
     */
}

void Hazama::loadInIFile()
{
    boost::property_tree::ptree config;

    //read
    boost::property_tree::read_ini("./../../procon2016-comp/config/threshold.ini",config);

    if(boost::optional<int> value_test = config.get_optional<int>("test.test")){

        std::cout << "test.test = " << value_test.get() << std::endl;

    }else{

        std::cerr << "cannot read config file" << std::endl;

    }



}

void Hazama::thresholdGUIinit()
{
    //set label
    ui->label_1->setText("kido");
    ui->label_2->setText("kaiaiai");
    ui->label_3->setText("iikannzi");
    ui->label_4->setText("yabai");

    //set 上限,下限
    ui->horizontalSlider_1->setRange(0,500);
    ui->horizontalSlider_2->setRange(0,500);
    ui->horizontalSlider_3->setRange(0,500);
    ui->horizontalSlider_4->setRange(0,500);

    //set stayle
    ui->horizontalSlider_1->setStyleSheet("QSlider::groove::horizontal {border: 1px solid #999999;height: 8px;background: white;margin: 2px 0;}QSlider::handle::horizontal{background: blue;border: red;width: 18px;width: 18px;border-radius: 3px;}");
    ui->horizontalSlider_2->setStyleSheet("QSlider::groove::horizontal {border: 1px solid #999999;height: 8px;background: white;margin: 2px 0;}QSlider::handle::horizontal{background: red;border: red;width: 18px;width: 18px;border-radius: 3px;}");
    ui->horizontalSlider_3->setStyleSheet("QSlider::groove::horizontal {border: 1px solid #999999;height: 8px;background: white;margin: 2px 0;}QSlider::handle::horizontal{background: green;border: red;width: 18px;width: 18px;border-radius: 3px;}");
    ui->horizontalSlider_4->setStyleSheet("QSlider::groove::horizontal {border: 1px solid #999999;height: 8px;background: white;margin: 2px 0;}QSlider::handle::horizontal{background: gray;border: red;width: 18px;width: 18px;border-radius: 3px;}");

    //if slideber`s value changed call thresholdValueChanged()
    connect(ui->horizontalSlider_1, &QSlider::valueChanged, this, &Hazama::thresholdValueChanged);
    connect(ui->horizontalSlider_2, &QSlider::valueChanged, this, &Hazama::thresholdValueChanged);
    connect(ui->horizontalSlider_3, &QSlider::valueChanged, this, &Hazama::thresholdValueChanged);
    connect(ui->horizontalSlider_4, &QSlider::valueChanged, this, &Hazama::thresholdValueChanged);

    //if applay button clicked value insert threshold class
    connect(ui->pushButton_1, &QPushButton::clicked, this, &Hazama::clickedApply_1_Button);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &Hazama::clickedApply_2_Button);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &Hazama::clickedApply_3_Button);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &Hazama::clickedApply_4_Button);

    ui->pushButton_1->setText("Apply");
    ui->pushButton_2->setText("Apply");
    ui->pushButton_3->setText("Apply");
    ui->pushButton_4->setText("Apply");


}

void Hazama::thresholdValueChanged()
{

    //get value
    int lcdNumber_1_value = ui->horizontalSlider_1->value();
    int lcdNumber_2_value = ui->horizontalSlider_2->value();
    int lcdNumber_3_value = ui->horizontalSlider_3->value();
    int lcdNumber_4_value = ui->horizontalSlider_4->value();

    //display lcdNumber
    ui->lcdNumber_1->display(lcdNumber_1_value);
    ui->lcdNumber_2->display(lcdNumber_2_value);
    ui->lcdNumber_3->display(lcdNumber_3_value);
    ui->lcdNumber_4->display(lcdNumber_4_value);

}


void Hazama::clickedApply_1_Button()
{

    //threshold::test_threshold_san = 99999;
    std::cout << "clicked_1_button" << std::endl;

}

void Hazama::clickedApply_2_Button()
{

    std::cout << "clicked_2_button" << std::endl;

}

void Hazama::clickedApply_3_Button()
{

    std::cout << "clicked_3_button" << std::endl;

}

void Hazama::clickedApply_4_Button()
{

    std::cout << "clicked_4_button" << std::endl;

}

void Hazama::enableThresholdUI()
{

    ui->horizontalSlider_1->setEnabled(true);
    ui->horizontalSlider_2->setEnabled(true);
    ui->horizontalSlider_3->setEnabled(true);
    ui->horizontalSlider_4->setEnabled(true);

    ui->pushButton_1->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_4->setEnabled(true);

}

void Hazama::disableThresholdUI()
{

    ui->horizontalSlider_1->setEnabled(false);
    ui->horizontalSlider_2->setEnabled(false);
    ui->horizontalSlider_3->setEnabled(false);
    ui->horizontalSlider_4->setEnabled(false);

    ui->pushButton_1->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);

}

