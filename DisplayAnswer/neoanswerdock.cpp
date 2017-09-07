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
    answer_board->setFixedSize(400, 400);
    this->ui->board_container->addWidget(answer_board, (fields.size() - 1 ) / 3, (fields.size() - 1 ) % 3);
}

//Add single piece
void NeoAnswerDock::makePieceList(const procon::NeoField &field)
{
    int i = 0;
    for(auto piece : field.getPieces()){
       NeoSinglePolygonDisplay* piece_list = new NeoSinglePolygonDisplay;
       piece_list->createInstance(piece.getPolygon(), "Piece", false);
       ui->board_container->addWidget(piece_list, i / 3, i % 3);
    }
}
