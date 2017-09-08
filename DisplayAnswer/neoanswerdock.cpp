#include "neoanswerdock.h"
#include "ui_neoanswerdock.h"

NeoAnswerDock::NeoAnswerDock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NeoAnswerDock)
{
    ui->setupUi(this);
    // Background image
    QImage image("../../procon2017-comp/background.png");
    QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsPixmapItem* pic = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->addItem(pic);
    ui->Picture->setScene(scene);
    this->setWindowTitle("Answer Progress");
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
    answer_board->setFixedSize(300, 300);
    this->ui->board_container->addWidget(answer_board, (fields.size() - 1 ) / 5, (fields.size() - 1 ) % 5);

    QEventLoop loop;
    QTimer::singleShot(2, &loop, SLOT(quit()));
    loop.exec();
}

//Add single piece
void NeoAnswerDock::makePieceList(const procon::NeoField &field)
{
    this->setWindowTitle("Piece List");
    this->ui->gridLayout_3->setSpacing(5);
    //this->show();

    int i = 0;

    for(auto piece : field.getElementaryPieces()){
       NeoPolygonViewer* piece_list = new NeoPolygonViewer;
       piece_list->getInstance().displayPolygon(piece.getPolygon(), "Piece", true);
       piece_list->setFixedSize(500, 500);
       this->ui->board_container->addWidget(piece_list, i / 3, i % 3);
       i++;
       this->update();
       std::cout << "i";
    }
}
