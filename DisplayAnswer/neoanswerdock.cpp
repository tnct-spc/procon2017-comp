#include "neoanswerdock.h"
#include "ui_neoanswerdock.h"

NeoAnswerDock::NeoAnswerDock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NeoAnswerDock)
{
    ui->setupUi(this);
    // Background image
    cv::Mat I = cv::imread("../../procon2017-comp/background.png");
    cv::cvtColor(I, I, CV_RGB2BGRA);
    QImage image(I.data, I.cols, I.rows, QImage::Format_RGBA8888);
    QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsPixmapItem* pic = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->addItem(pic);
    ui->Picture->setScene(scene);
}

NeoAnswerDock::~NeoAnswerDock()
{
    delete ui;
}

//Add piece and frame
void NeoAnswerDock::addAnswer(const procon::NeoField &field)
{
    fields.push_back(field);
    NeoAnswerBoard* answer_board = new NeoAnswerBoard();
    answer_board->setField(field);
    answer_board->setSingleMode(true);
//  size setting
    answer_board->setFixedSize(305, 300);

//  remove texts
        if(c == 1)
            this->ui->tab1T->hide();
        else if(c == 10001)
            this->ui->tab2T->hide();
        else if(c == 20001)
            this->ui->tab3T->hide();
        else if(c == 30001)
            this->ui->tab4T->hide();
        else if(c == 40001)
            this->ui->tab5T->hide();

//  select tab
    if(c <= 10000){
        this->ui->board_container->addWidget(answer_board, (fields.size() - 1 ) / 5, (fields.size() - 1 ) % 5);
    }
    else if(c <= 20000){
        this->ui->board_container_2->addWidget(answer_board, (fields.size() - 1 ) / 5, (fields.size() - 1 ) % 5);
    }
    else if(c <= 30000){
        this->ui->board_container_3->addWidget(answer_board, (fields.size() - 1 ) / 5, (fields.size() - 1 ) % 5);
    }
    else if(c <= 40000){
        this->ui->board_container_4->addWidget(answer_board, (fields.size() - 1 ) / 5, (fields.size() - 1 ) % 5);
    }
    else if(c <= 50000){
        this->ui->board_container_5->addWidget(answer_board, (fields.size() - 1 ) / 5, (fields.size() - 1 ) % 5);
    }
    c++;

    QEventLoop loop;
    QTimer::singleShot(1, &loop, SLOT(quit()));
    loop.exec();
}

//Make piece list
void NeoAnswerDock::makePieceList(procon::NeoField field)
{
//  Window settings
    this->setWindowTitle("Piece List");
    this->ui->gridLayout_3->setSpacing(5);
    this->ui->label->setText("Piece List");
    QFont font = ui->label->font();
    font.setPointSize(46);
    this->ui->label->setFont(font);
    this->ui->label_2->hide();
    this->ui->tab1T->hide();
    this->show();
    int i = 0;

//  make piece list
    for(auto piece : field.getElementaryPieces()){
       NeoSinglePolygonDisplay* piece_list = new NeoSinglePolygonDisplay();
       piece_list->setPolygon(piece.getPolygon());
       piece_list->setIsEnlargedPolygon(false);
       piece_list->setFixedWidth(520);
       piece_list->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
       piece_list->setFixedHeight(1000);
       this->ui->board_container->addWidget(piece_list , i / 3, i % 3);
       i++;
    }
}
