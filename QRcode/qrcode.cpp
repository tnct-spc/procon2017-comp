//新世代
#include "qrcode.h"
#include "ui_qrcode.h"

using namespace cv;
using namespace zbar;

QRCode::QRCode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QRCode)
{
    ui->setupUi(this);
}

QRCode::~QRCode()
{
    delete ui;
}

std::pair<std::vector<polygon_i>,std::vector<polygon_i>> QRCode::Decoder(bool s, bool is_hint, bool hint_multi, int how_qr)
{
    VideoCapture cap(1);
    if (!cap.isOpened())  // if not success, exit program
    {
        std::cout << "Cannot open the video cam" << std::endl;
        while(!cap.isOpened()){

        }
    }

    int resizedW;
    std::pair<std::vector<polygon_i>,std::vector<polygon_i>> decoded_polygons;
    std::string code = {""};
    std::string bcode = {"."};
    std::string type = {""};
    int much = 0;

    ImageScanner scanner;
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

    double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    std::cout << "Camera size : " << dWidth << " x " << dHeight << std::endl;

    resizedW = (dWidth - dHeight) / 2;

    while (1)
    {
        Mat srcframe;

        bool bSuccess = cap.read(srcframe); // read a new frame from video

        if (!bSuccess) //if not success, break loop
        {
            std::cout << "Cannot read a frame from video stream" << std::endl;
            break;
        }
        Mat frame(srcframe, Rect(resizedW, 0, dHeight, dHeight)); //Resize
        Mat grey;
        cvtColor(frame,grey,CV_BGR2GRAY);

        int width = frame.cols;
        int height = frame.rows;
        uchar *raw = (uchar *)grey.data;
        // wrap image data
        Image image(width, height, "Y800", raw, width * height);
        // scan the image for barcodes
        int n = scanner.scan(image);
        // extract results
        for(Image::SymbolIterator symbol = image.symbol_begin();
            symbol != image.symbol_end();
            ++symbol) {
            std::vector<Point> vp;
            // do something useful with results
            bcode = code;
            code = symbol->get_data();
            type = symbol->get_type_name();
            std::cout << "decoded " << symbol->get_type_name()  << " symbol \"" << symbol->get_data() << '"' <<" "<< std::endl;
            int n = symbol->get_location_size();
            for(int i=0;i<n;i++){
                vp.push_back(Point(symbol->get_location_x(i),symbol->get_location_y(i)));
            }
            RotatedRect r = minAreaRect(vp);
            Point2f pts[4];
            r.points(pts);
            for(int i=0;i<4;i++){
                line(frame,pts[i],pts[(i+1)%4],Scalar(255,0,0),3);
            }
            std::cout << "Angle: " << r.angle << std::endl;
            QrTranslateToPolygon qrtrans(code);
            is_multi = qrtrans.getIsMultiQr();
            if(bcode != code){
                std::vector<polygon_i> pieceData = qrtrans.getPieceData();
                std::vector<polygon_i> frameData = qrtrans.getFrameData();
                for(auto p : pieceData){
                    decoded_polygons.first.push_back(p);
                    std::cout << "piece: " << decoded_polygons.first.size() << std::endl;
                }

                for(auto f : frameData){
                    decoded_polygons.second.push_back(f);
                    std::cout << "frame: " << decoded_polygons.second.size() << std::endl;
                }
                much++;
            }
        }

        imshow("QR", frame);
        int key = waitKey(1);
        if(code.length() > 2 && s){
            if(is_hint){
                if(hint_multi){
                    if(much == how_qr){
                        std::cout << "QRCode detected" << std::endl;
                        if(!read) QrTranslateToPolygon::translateToCSV(decoded_polygons, is_hint);
                        read = true;
                        std::cout << "a: " << decoded_polygons.first.size() << std::endl;
                        std::cout << "b: " << decoded_polygons.second.size() << std::endl;
                        break;
                    }
                }
            }else if(!is_multi){
                std::cout << "QRCode detected" << std::endl;
                if(!read) QrTranslateToPolygon::translateToCSV(decoded_polygons, is_hint);
                read = true;
                std::cout << "a: " << decoded_polygons.first.size() << std::endl;
                std::cout << "b: " << decoded_polygons.second.size() << std::endl;
                destroyAllWindows();
                break;
            }
        }
    }
//    code = "Type: " + type + "\n\n\"" + code + "\"";
//    return code;
    return decoded_polygons;
}
