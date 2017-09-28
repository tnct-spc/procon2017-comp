#include "manpowerprob.h"
#include "ui_manpowerprob.h"

ManPowerProb::ManPowerProb(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ManPowerProb)
{
    ui->setupUi(this);


}

ManPowerProb::~ManPowerProb()
{
    delete ui;
}

void ManPowerProb::paintEvent(QPaintEvent *event){

    QPainter painter(this);
    const QString back_ground_color = "#EEEEEE";
    const int window_width = this->width();
    const int window_height = this->height();
    // 101 x 65
    const int grid_row = 65;
    const int grid_col = 101;
    const int grid_margin = 1;
    const int splitedheight = window_height;
    grid_size =
            window_width <= window_height
            ? window_width / (grid_col + grid_margin)
            : splitedheight / (grid_row + grid_margin);
    //grid_size -= grid_size*8;
    top_bottom_margin = (splitedheight - grid_size * grid_row) / 2;
    left_right_margin = (window_width - grid_size * grid_col) / 2;

    painter.setBrush(QBrush(QColor(back_ground_color)));
    painter.drawRect(QRect(0,0,window_width,window_height));

    auto drawGrid = [&]{
        painter.setPen(QPen(QBrush(Qt::black),0.5));
        for (int current_col = 0; current_col < grid_col + 1; ++current_col) {
            int x = current_col * grid_size + left_right_margin;
            painter.drawLine(x,top_bottom_margin,x,splitedheight - top_bottom_margin);
        }
        for (int current_row = 0; current_row < grid_row + 1; ++current_row) {
            int y = current_row * grid_size + top_bottom_margin;
            painter.drawLine(left_right_margin,y,window_width - left_right_margin,y);
        }
    };



    drawGrid();
}
void ManPowerProb::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::MouseButton::LeftButton){
        std::vector<procon::NeoExpandedPolygon> neopieces;
        std::vector<procon::NeoExpandedPolygon> neoframes;

        procon::NeoExpandedPolygon neopiece;
        for(auto piece : pieces){
            neopiece.resetPolygonForce(piece);
            neopieces.push_back(neopiece);
        }
        procon::NeoExpandedPolygon neoframe;
        for(auto frame : frames){
            neoframe.resetPolygonForce(frame);
            neoframes.push_back(neoframe);
        }
        field.setFrame(neoframes);
        field.setElementaryPieces(neopieces);
    }
}

void ManPowerProb::keyPressEvent(QKeyEvent *event){

    if(event->key() == Qt::Key_Enter){//EnterでexportCSV

        NeoPolygonIO::exportPolygon(this->field,"../humanpowerfield.csv");
    }
}

QPointF ManPowerProb::getPosition(point_i point){//point_iを上画面のgridと対応させるようにQPointFに変換する
    int pointx = point.x();
    int pointy = point.y();
    return QPointF(left_right_margin + pointx * grid_size, top_bottom_margin + pointy * grid_size);
}
