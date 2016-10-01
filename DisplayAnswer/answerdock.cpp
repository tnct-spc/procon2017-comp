#include "answerdock.h"
#include "ui_answerdock.h"

#include "answerboard.h"
#include "field.h"
#include <QScrollArea>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>

AnswerDock::AnswerDock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnswerDock)
{
    ui->setupUi(this);
    QImage image("../../procon2016-comp/yurikamome.jpg");
    QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsPixmapItem* pic = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->addItem(pic);
    ui->Picture->setScene(scene);
}

AnswerDock::~AnswerDock()
{
    delete ui;
}

cv::Mat AnswerDock::raw_pieces_pic;
std::vector<cv::Point> AnswerDock::raw_pieces_pos;
std::vector<cv::Vec3b> AnswerDock::raw_random_colors;

void AnswerDock::setupDefaultRawPicData(const cv::Mat& raw_pieces_pic_, const std::vector<cv::Point>& raw_pieces_pos_, const std::vector<cv::Vec3b>& raw_random_colors_)
{
    raw_pieces_pic = raw_pieces_pic_;
    raw_pieces_pos = raw_pieces_pos_;
    raw_random_colors = raw_random_colors_;
}

void AnswerDock::addAnswer(procon::Field const& field)
{
    AnswerBoard* answer_board = new AnswerBoard();
    answer_board->setRawPicture(raw_pieces_pic, raw_pieces_pos);
    answer_board->setRandomColors(raw_random_colors);
    answer_board->setField(field);
    answer_board->SINGLE_MODE = true;
    answer_board->setFixedSize(300,300);

    static int view_count = 0;
    this->ui->testd->addWidget(answer_board,view_count/4,view_count%4);
    view_count++;
}
