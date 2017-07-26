#include "neoanswerboard.h"
#include "ui_neoanswerboard.h"

NeoAnswerBoard::NeoAnswerBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NeoAnswerBoard)
{
    ui->setupUi(this);
}

NeoAnswerBoard::~NeoAnswerBoard()
{
    delete ui;
}
QRgb NeoAnswerBoard::NeosetRandomColors()
{
    std::random_device rnd;
    std::uniform_int_distribution<int> rand255;
    QRgb randomcolors = (static_cast<void>(rand255(rnd)), static_cast<void>(rand255(rnd)), static_cast<void>(rand255(rnd)), 255);
    return randomcolors;

}

void NeoAnswerBoard::beforePolygon()
{
    //format
    //const int picture_width;
    //const int picture_height;
    const int window_width = this->width();
    const int window_height = this->height();
    const int app_height = window_height / 2;
    //const int top_bottom_margin;
}

void NeoAnswerBoard::paintEvent(QPaintEvent *event)
{
    const QString back_ground_color = "#00FFFF";
    const int window_width = this->width();
    const int window_height = this->height();

    // 101 x 65
    const int grid_row = 65;
    const int grid_col = 101;
    const int grid_margin = 1;
    const int splitedheight = window_height/2;
    grid_size =
            window_width <= splitedheight
            ? window_width / (grid_col + grid_margin)
            : splitedheight / (grid_row + grid_margin);
    top_bottom_margin = (splitedheight - grid_size * grid_row) / 2;
    left_right_margin = (window_width - grid_size * grid_col) / 2;

    top_margin = grid_size * grid_margin;
    QPainter painter(this);

    //draw background
    painter.setBrush(QBrush(QColor(back_ground_color)));
    painter.drawRect(QRect(0,0,window_width,window_height/2));


    //draw grid
    auto drawGrid = [&]{
        painter.setPen(QPen(QBrush(Qt::black),0.1));
        const int splitedheight = window_height/2;
        grid_size =
                window_width <= splitedheight
                ? window_width / (grid_col + grid_margin)
                : splitedheight / (grid_row + grid_margin);
        const int top_bottom_margin = (splitedheight - grid_size * grid_row) / 2;
        const int left_right_margin = (window_width - grid_size * grid_col) / 2;
        for (int current_col = 0; current_col < grid_col + 1; ++current_col) {
            int x = current_col * this->grid_size + left_right_margin;
            painter.drawLine(x,top_bottom_margin,x,splitedheight - top_bottom_margin);
        }
        for (int current_row = 0; current_row < grid_row + 1; ++current_row) {
            int y = current_row * grid_size + top_bottom_margin;
            painter.drawLine(left_right_margin,y,window_width - left_right_margin,y);
        }
    };

    auto testColors = [&]{


        static const QPointF points[4] = {
            QPointF(10.0, 80.0),
            QPointF(20.0, 10.0),
            QPointF(80.0, 30.0),
            QPointF(90.0, 70.0)
        };
        painter.drawPolygon(points, 4);
        static const QPointF pointss[4] = {
            QPointF(100.0, 400.0),
            QPointF(200.0, 100.0),
            QPointF(800.0, 300.0),
            QPointF(900.0, 700.0)
        };
        painter.drawPolygon(pointss, 4);
    };
    drawGrid();
    testColors();
    auto setField = [&]{
        procon::ExpandedPolygon polygon;
        procon::ExpandedPolygon poly0;
        procon::ExpandedPolygon poly1;
        procon::ExpandedPolygon poly2;
        std::vector<polygon_t> piecepolygon(3);
        polygon_t framepolygon;

        framepolygon.outer().push_back(point_t(5,5));
        framepolygon.outer().push_back(point_t(85,7));
        framepolygon.outer().push_back(point_t(75,48));
        framepolygon.outer().push_back(point_t(18,57));
        framepolygon.outer().push_back(point_t(5,5));
        polygon.resetPolygonForce(framepolygon);
        field.setFrame(polygon);

        piecepolygon[0].outer().push_back(point_t(5,5));
        piecepolygon[0].outer().push_back(point_t(45,6));
        piecepolygon[0].outer().push_back(point_t(25,15));
        piecepolygon[0].outer().push_back(point_t(5,5));
        polygon.resetPolygonForce(piecepolygon[0]);
        field.setPiece(polygon);

        piecepolygon[1].outer().push_back(point_t(45,6));
        piecepolygon[1].outer().push_back(point_t(65,36));
        piecepolygon[1].outer().push_back(point_t(45,35));
        piecepolygon[1].outer().push_back(point_t(45,6));
        polygon.resetPolygonForce(piecepolygon[1]);
        field.setPiece(polygon);

        piecepolygon[2].outer().push_back(point_t(12,32));
        piecepolygon[2].outer().push_back(point_t(15,21));
        piecepolygon[2].outer().push_back(point_t(35,23));
        piecepolygon[2].outer().push_back(point_t(44,35));
        piecepolygon[2].outer().push_back(point_t(32,45));
        piecepolygon[2].outer().push_back(point_t(7,12));
        polygon.resetPolygonForce(piecepolygon[2]);
        field.setPiece(polygon);

    };
    auto drawFrame = [&]{
        painter.setBrush(QBrush(QColor(236,182,138))); //frame color
            painter.setPen(QPen(QBrush(Qt::black),0.5));
        painter.drawRect(QRect(left_right_margin,top_bottom_margin,grid_col*grid_size,grid_row*grid_size));
        QPointF points[4];
        for(int tes = 0;tes < 4; tes++){
            points[tes] = getPosition(field.getFrame().getPolygon().outer().at(tes));
        }
        painter.setBrush(QBrush(QColor(back_ground_color)));
        painter.drawPolygon(points,4);
    };
    auto drawPiece = [&]{
        painter.setBrush(QBrush(QColor(126,12,228))); //random color
        for(unsigned int pnum =0; pnum < field.getPieces().size(); pnum++){
            int pcount = field.getPiece(pnum).getSize();
            QPointF points[pcount];
            for(int tes = 0;tes < pcount; tes++){
                points[tes] = getPosition(field.getPiece(pnum).getPolygon().outer().at(tes));
            }
            painter.drawPolygon(points,pcount);
        }
    };
    auto drawPieceId = [&]{
      painter.setPen(QPen(QColor(10,250,10))); //text color
      painter.setFont(QFont("Decorative", grid_size*4, QFont::Bold)); // text font
        for(unsigned int pnum = 0; pnum < field.getPieces().size(); pnum++){
            //get polygon center
            point_t center = {0,0};
           // boost::geometry::centroid(field.getPiece(pnum),center);

            QPointF piececenter = getPosition(field.getPiece(pnum).getPolygon().outer().at(0));
            //painter.drawText(piececenter, QString(QString::number(field.getPiece(pnum).getId())));
            painter.drawText(piececenter, QString::fromStdString(field.getPiece(pnum).makeMultiIdString())); //cannnot use now
        }
    };
    setField();
    drawFrame();
    drawPiece();
    drawPieceId();
    drawGrid();
}
QPointF NeoAnswerBoard::getPosition(point_t point){
    return QPointF(left_right_margin + point.x() * grid_size, top_bottom_margin + point.y() * grid_size);
}
