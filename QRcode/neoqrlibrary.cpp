//This is new QRLibrary.

#include "neoqrlibrary.h"
#include "qrtranslatetopolygon.h"

using namespace cv;
using namespace zbar;

NeoQRLibrary::NeoQRLibrary()
{
    std::cout << "new QRLibrary initialized" << std::endl;
}

std::pair<std::vector<polygon_i>, polygon_i> NeoQRLibrary::Decoder(bool s)
{
    VideoCapture cap(0);
    std::string code = "";
    std::string type = "";

    cap.set(CV_CAP_PROP_FRAME_WIDTH, 600);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 600);

    if (!cap.isOpened())
    {
        std::cout << "Cannot open the video cam" << std::endl;
    }

    ImageScanner scanner;
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

    double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    std::cout << "Camera size: " << dWidth << " x " << dHeight << std::endl;

    namedWindow("Press Esc key to exit", CV_WINDOW_NORMAL);

    std::pair<std::vector<polygon_i>, polygon_i> decoded_polygons;

    while(1){
        Mat frame;

        bool is_success = cap.read(frame);

        if(!is_success){
            std::cout << "Can't read a frame from video stream" << std::endl;
            break;
        }

        Mat gray;
        cvtColor(frame, gray, CV_BGR2GRAY);

        int width = frame.cols;
        int height = frame.rows;
        uchar *raw = (uchar*)gray.data;
        Image image(width, height, "Y800", raw, width * height);
        int n = scanner.scan(image);
        for(Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol) {
            std::vector<Point> vp;
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
            imshow("Press Esc key to exit", frame);
        }
    }

    int key = waitKey(30);
    return decoded_polygons;
}
