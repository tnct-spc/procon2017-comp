#include "hazama.h"
#include "ui_hazama.h"

#include "polygonviewer.h"

Hazama::Hazama(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Hazama)
{
    ui->setupUi(this);
    connect(ui->RunButton, &QPushButton::clicked, this, &Hazama::clickedRunButton);

    // ConfigGUI
    connect(ui->LoadConfig, &QPushButton::clicked, this, &Hazama::loadInIFile);
    connect(ui->WriteConfig, &QPushButton::clicked, this, &Hazama::makeInIFile);

    // Answer board
    board = std::make_shared<AnswerBoard>();
    board->showMaximized();

    // Server
    QObject::connect(&request_mapper,SIGNAL(getAnswer(QString)),this,SLOT(acceptAnswer(QString)));

    thresholdGUIinit();

    // Threshold
    this->ui->horizontalSlider_1->setValue(threshold::LSDthrehold*100);
}

Hazama::~Hazama()
{
    delete ui;
}

void Hazama::pauseClicked()
{
    is_pause_flag = true;
}

void Hazama::startClicked()
{
    board->setField(best_answer);
    is_pause_flag = false;
}

void Hazama::acceptAnswer(QString file_path)
{
    std::cout<<"accept new answer"<<std::endl;

    // Accept POST puzzle answer data
    procon::Field field = procon::PolygonIO::importAnswer(file_path.toStdString(),PDATA);

    //Display answer
    emitAnswer(field);
}

void Hazama::emitAnswer(procon::Field field)
{
    std::cout << "emitted answer" << std::endl;
    if(!ui->isKeepBestAnswerMode->isChecked() || first_answer_flag){
        first_answer_flag = false;
        best_answer = field;
    }else{
        if(field.getFrame().getJointedPieces().size() > best_answer.getFrame().getJointedPieces().size()){
            best_answer = field;
        }
    }

    field_stack.push_back(field);
    field_stack_pointer++;

    //Display answer
    // Wait 1msec
    QEventLoop loop;
    QTimer::singleShot(1, &loop, SLOT(quit()));
    loop.exec();
    if(!is_pause_flag){
        board->setField(best_answer);
    }
    //PolygonViewer::getInstance().pushPolygon(field.getFrame(),std::string("Answer Frame"));
}

void Hazama::leftClicked()
{
    if(field_stack_pointer != 1){
        field_stack_pointer--;
    }
    board->setField(field_stack.at(field_stack_pointer-1));
    //Display answer
    // Wait 1msec
    QEventLoop loop;
    QTimer::singleShot(1, &loop, SLOT(quit()));
    loop.exec();
}

void Hazama::rightClicked()
{
    if(field_stack_pointer != field_stack.size()){
        field_stack_pointer++;
    }
    board->setField(field_stack.at(field_stack_pointer-1));
    //Display answer
    // Wait 1msec
    QEventLoop loop;
    QTimer::singleShot(1, &loop, SLOT(quit()));
    loop.exec();
}

void Hazama::makeCalibrationData(std::string savefile_path,unsigned int numberOfImages)
{
    // Reference : http://yuki-sato.com/wordpress/2016/04/15/opencv-%E3%82%AB%E3%83%A1%E3%83%A9%E3%81%AE%E6%AD%AA%E3%81%BF%E3%82%92%E3%81%AA%E3%81%8A%E3%81%99%E3%82%AD%E3%83%A3%E3%83%AA%E3%83%96%E3%83%AC%E3%83%BC%E3%82%B7%E3%83%A7%E3%83%B3-c/

    cv::namedWindow("debugwindow", CV_WINDOW_AUTOSIZE);

    int horizonalCrossCount = 10;
    int verticalCrossCount = 7;

    // Calibration points
    std::vector<std::vector<cv::Point3f>> object_points;
    std::vector<std::vector<cv::Point2f>> image_points;
    std::vector<cv::Point3f> obj;
    for(int j=0;j< horizonalCrossCount * verticalCrossCount;j++) {
        obj.push_back(cv::Point3f(j/horizonalCrossCount, j%horizonalCrossCount, 0.0f));
    }

    for(unsigned int i=0; i<numberOfImages; i++) {

        std::cout << "now:" << i << std::endl;

        char filename[128];
        sprintf(filename, "../../procon2017-comp/picture/cal/pic%d.JPG", i);
        cv::Mat frame = cv::imread(filename);
        cv::Mat gray;

        cv::cvtColor(frame, gray, CV_BGR2GRAY);

        // 10-7チェスを探す
        cv::Size chessboardPatterns(horizonalCrossCount, verticalCrossCount);
        std::vector<cv::Point2f> centers;
        bool found = cv::findChessboardCorners(gray, chessboardPatterns, centers, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);
        if (found) {
            // 見つけたpointを高精度化する
            cv::cornerSubPix(gray, centers, cv::Size(11,11), cv::Size(-1,-1), cv::TermCriteria (cv::TermCriteria::EPS+cv::TermCriteria::COUNT, 30, 0.1));
            object_points.push_back(obj);
            image_points.push_back(centers);

        } else {
            std::cout << "error! chess board not found" << std::endl;
        }
    }

    // Calibrate
    std::vector<cv::Mat> rvecs, tvecs;
    cv::Mat mtx, dist;
    cv::calibrateCamera(object_points, image_points, cv::Size(1920,1080), mtx, dist, rvecs, tvecs);

    // Save
    cv::FileStorage fs(savefile_path, cv::FileStorage::WRITE);
    fs << "mtx" << mtx;
    fs << "dist" << dist;
    fs.release();
}

void Hazama::run()
{
    // When you want to calibrate webcamera,please comment out this line!!!
    //makeCalibrationData("./../../procon2017-comp/picture/cal/calibration.yml",NUM);

    std::cout << "Run" << std::endl;

    // ピースの画像
    cv::Mat nocpieces;
    cv::Mat nocframe;

    // Disable threshold UI
    disableThresholdUI();

    std::string frame_path = ui->franeURL->text().toStdString();
    std::string pieces_path = ui->pieceURL->text().toStdString();
    std::string path = "./../../procon2017-comp/sample/data.csv";

    // Get puzzle data
    if(ui->useWebCamera->isChecked() || ui->useImageData->isChecked() || ui->selectImageData->isChecked()){
        cv::Mat raw_frame;
        cv::Mat raw_pieces;
        // Get Image
        if(ui->useWebCamera->isChecked()){

            // SlectWebCamra device number
            device_number = selectWebCamera();

            // Camera open
            capture(device_number);

            cv::namedWindow("capture",cv::WINDOW_AUTOSIZE);
            cv::Mat press_enter_text_window = cv::Mat::zeros(100,700,CV_8UC3);
            cv::putText(press_enter_text_window, "Please Press Enter",cv::Point(0,100),cv::FONT_HERSHEY_SCRIPT_SIMPLEX,2.4,cv::Scalar(255,255,255),2,CV_AA);
            cv::Mat press_enter_text_second_window = cv::Mat::zeros(100,700,CV_8UC3);
            cv::putText(press_enter_text_second_window,"Please Press Enter",cv::Point(0,100),cv::FONT_HERSHEY_SCRIPT_SIMPLEX,2.4,cv::Scalar(0,255,255),2,CV_AA);
            cv::imshow("capture",press_enter_text_window);
            while(cv::waitKey(0)==13);
            raw_frame = capture(device_number);
            cv::imshow("capture",press_enter_text_second_window);
            while(cv::waitKey(0)==13);
            raw_pieces = capture(device_number);
            cv::imwrite("frame.png",raw_frame,std::vector<int>(CV_IMWRITE_PNG_COMPRESSION,0));
            cv::imwrite("pieces.png",raw_pieces,std::vector<int>(CV_IMWRITE_PNG_COMPRESSION,0));
            cv::destroyWindow("capture");

        } else {

            //read

            if(ui->selectImageData->isChecked()){
                //環境によっては動かない
                frame_path = QFileDialog::getOpenFileName(this,"input frame picture","/media/spc").toStdString();
                pieces_path = QFileDialog::getOpenFileName(this,"input pieces picture","/media/spc").toStdString();
            }
            nocframe = cv::imread(frame_path, 1);
            nocpieces = cv::imread(pieces_path, 1);

            //calibration
            cv::Mat mtx,dist;
            cv::FileStorage fs(calibration_data_file_path, cv::FileStorage::READ);
            fs["mtx"] >> mtx;
            fs["dist"] >> dist;
            fs.release();
            cv::undistort(nocframe, raw_frame, mtx, dist);
            cv::undistort(nocpieces, raw_pieces, mtx, dist);
        }

        // Image detect
//        PDATA = imrec.run(raw_frame, nocpieces);

        // Set raw image data
        AnswerBoard::setRawPicture(imrec.getRawPiecesPic(), imrec.getRawPiecesPos());
        AnswerBoard::setRandomColors(imrec.getRawRandomColors());

    } else if (ui->useFileData->isChecked()) {
//        PDATA = procon::PolygonIO::importPolygon(path);

    } else {
        return;

    }

    // Hide HAZAMA
    //this->showMinimized();

    if(ui->server_mode_radio->isChecked()){
        // Save Puzzle
        std::string PROBLEM_SAVE_PATH = QCoreApplication::applicationDirPath().toStdString()+"/docroot/problem.csv";
        std::cout<<"Save problem in : "<<PROBLEM_SAVE_PATH<<std::endl;
        procon::PolygonIO::exportPolygon(PDATA, PROBLEM_SAVE_PATH);
    }else{
        // Solve puzzle
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
            // Poa
        }
        solver->run(PDATA, algorithm_number);
    }



    // Enable thresholdUI
    enableThresholdUI();

    std::cout<<"Finish"<<std::endl;
}

cv::Mat Hazama::capture(int deviceNumber)
{
    // Init
    static cv::Mat mtx,dist;
    static bool init_calibration_flag = false;
    if(!init_calibration_flag){
        init_calibration_flag = true;

        cv::FileStorage fs(calibration_data_file_path, cv::FileStorage::READ);
        fs["mtx"] >> mtx;
        fs["dist"] >> dist;
        fs.release();
    }

    //デバイスのオープン
    cv::VideoCapture cap(deviceNumber);
    if(!cap.isOpened()){
        std::cerr << "Couldn't open camera!" << std::endl;
    }

    // Camera setting
    cap.set(cv::CAP_PROP_FRAME_WIDTH,1920);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT,1080);
    cap.set(cv::CAP_PROP_BRIGHTNESS,0.3);
    cap.set(cv::CAP_PROP_FOCUS,0.3);
    cap.set(cv::CAP_PROP_SATURATION,80);
    cap.set(cv::CAP_PROP_BACKLIGHT,20);

    // Capture
    cv::Mat src;
    for(int i=0;i<30;i++){ // Too slow...
        cap >> src;
    }

    // Calibrate
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

        return 0;

    } else if(MsgBox.clickedButton() == button1) {

        return 1;

    }

    MsgBox.deleteLater();

    return 0;
}

void Hazama::makeInIFile()
{
    boost::property_tree::ptree config;

    // Put
    config.put("test.test",13);

    // Write
    boost::property_tree::write_ini("./../../procon2017-comp/config/threshold.ini",config);

    /* threhsold.ini write example
     *
     * [test]
     * test=13
     */
}

void Hazama::loadInIFile()
{
    boost::property_tree::ptree config;

    // Read
    boost::property_tree::read_ini("./../../procon2017-comp/config/threshold.ini",config);

    if(boost::optional<int> value_test = config.get_optional<int>("test.test")){

        std::cout << "test.test = " << value_test.get() << std::endl;

    }else{

        std::cerr << "cannot read config file" << std::endl;

    }
}

void Hazama::thresholdGUIinit()
{
    //set label
    ui->label_1->setText("LSD Threhosld");
    ui->label_2->setText("kaiaiai");
    ui->label_3->setText("iikannzi");
    ui->label_4->setText("yabai");

    //set 上限,下限
    ui->horizontalSlider_1->setRange(0,100);
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
    double lcdNumber_1_value = static_cast<double>(ui->horizontalSlider_1->value()) / 100;
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
    std::cout << "clicked_apply_1_button" << std::endl;
    threshold::LSDthrehold = static_cast<double>(ui->horizontalSlider_1->value()) /100;
    //
    std::cout << "LSDthrehold set to " << threshold::LSDthrehold << std::endl;

}

void Hazama::clickedApply_2_Button()
{

    std::cout << "clicked_apply_2_button" << std::endl;

}

void Hazama::clickedApply_3_Button()
{

    std::cout << "clicked_apply_3_button" << std::endl;

}

void Hazama::clickedApply_4_Button()
{

    std::cout << "clicked_apply_4_button" << std::endl;

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

