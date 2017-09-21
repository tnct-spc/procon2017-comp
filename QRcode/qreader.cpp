#include "qreader.h"
#include "ui_qreader.h"

using namespace cv;
using namespace zbar;

QReader::QReader(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::QReader)
{
  ui->setupUi(this);
  std::cout << "QReader initialized" << std::endl;
  connect(ui->buttonExit, SIGNAL(clicked()), this, SLOT(Quit()));
  connect(ui->buttonDone, SIGNAL(clicked()), this, SLOT(close()));
  this->show();
}

std::string QReader::Decoder()
{
  int resizedW;
  std::string code = {""};
  std::string type = {""};
  VideoCapture cap(0); // open the video camera no. 0

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

  std::pair<std::vector<polygon_i>,polygon_i> decoded_polygons;

  while (c)
  {
      Mat srcframe;
      std::string temp = {""};
      std::string comp = {"0"};

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
          temp = symbol->get_data();
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
          for(unsigned int tes=0;tes<qrtrans.getPieceData().size();tes++){
              std::cout << "polygon:" << bg::dsv(qrtrans.getPieceData()[tes]) << std::endl;
          }
          std::cout << "frame" << bg::dsv(qrtrans.getFrameData()) << std::endl;

          for(auto polygon : qrtrans.getPieceData()){
              decoded_polygons.first.push_back(polygon);
          }
          for(auto point: qrtrans.getFrameData().outer()){
              decoded_polygons.second.outer().push_back(point);
          }
      }

      cvtColor(frame, frame, CV_RGB2BGRA);
      QImage viewer(frame.data, frame.cols, frame.rows, QImage::Format_RGBA8888);
      QGraphicsScene* scene = new QGraphicsScene();
      QGraphicsPixmapItem* pic = new QGraphicsPixmapItem(QPixmap::fromImage(viewer));
      scene->addItem(pic);
      ui->display->setScene(scene);

      int key = waitKey(30);
      if(temp.length() > 2 && comp != temp){
          std::cout << "QRCode detected" << std::endl;
          comp = temp;
          code_count++;
          this->ui->lcdNumber->display(code_count);
      }
      if(key == 27){
          std::cout << "Esc key was pressed" << std::endl;
          break;
      }
      code = "\n\n" + temp;
  }
  cap.release();
//    code = "Type: " + type + "\n\n\"" + code + "\"";
  return code;
//    return decoded_polygons;
}

void QReader::stop()
{
    c = 0;
}

QReader::~QReader()
{
  delete ui;
}

void QReader::Quit()
{
  exit(0);
}
