#include "qrtranslatetopolygon.h"
#include "qrlibrary.h"

using namespace cv;
using namespace zbar;

QRLibrary::QRLibrary()
{
    std::cout << "QR Libruary initialized" << std::endl;
}

std::pair<std::vector<polygon_i>,std::vector<polygon_i>> QRLibrary::Decoder(bool s)
{
    int resizedW;
    std::string code = {""};
    std::string type = {""};
    VideoCapture cap(1); // open the video camera no. 0

    if (!cap.isOpened())  // if not success, exit program
    {
        std::cout << "Cannot open the video cam" << std::endl;
    }

    ImageScanner scanner;
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

    double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    std::cout << "Camera size : " << dWidth << " x " << dHeight << std::endl;

    resizedW = (dWidth - dHeight) / 2;

    namedWindow("Press Esc to exit",CV_WINDOW_NORMAL);
    if(dHeight < 600){
        resizeWindow("Press Esc to exit", 600, 600);
    }

    std::pair<std::vector<polygon_i>,std::vector<polygon_i>> decoded_polygons;

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
            if(is_pressed_enter){
            std::cout << "Angle: " << r.angle << std::endl;
            QrTranslateToPolygon qrtrans(code);
            is_multi = qrtrans.getIsMultiQr();
            std::vector<polygon_i> pieceData = qrtrans.getPieceData();
            std::vector<polygon_i> frameData = qrtrans.getFrameData();
            decoded_polygons.first = pieceData;
            decoded_polygons.second = frameData;
            QrTranslateToPolygon::translateToCSV(pieceData, frameData);
            }
        }

        imshow("Press esc key to exit", frame);

        int key = waitKey(1);
        if(!is_multi && code.length() > 2 && s){
            std::cout << "QRCode detected" << std::endl;
            break;
        }
    }

//    code = "Type: " + type + "\n\n\"" + code + "\"";
//    return code;
    return decoded_polygons;
}

void QRLibrary::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter && event->type() == QEvent::KeyPress){
        is_pressed_enter = true;
    }
    if(event->key() == Qt::Key_Enter && event->type() == QEvent::KeyRelease){
        is_pressed_enter = false;
    }
}
