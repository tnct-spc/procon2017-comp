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
    int S = 260;
    fields.push_back(field);
    NeoAnswerBoard* answer_board = new NeoAnswerBoard();
    answer_board->setField(field);
    answer_board->setSingleMode(true);
    answer_board->setFixedSize(S, 300);
    this->ui->board_container->addWidget(answer_board, (fields.size() - 1 ) / 6, (fields.size() - 1 ) % 6);

    QEventLoop loop;
    QTimer::singleShot(2, &loop, SLOT(quit()));
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
    this->show();
    int i = 0;

    for(auto piece : field.getElementaryPieces()){
       NeoSinglePolygonDisplay* piece_list = new NeoSinglePolygonDisplay();
       piece_list->setPolygon(piece.getPolygon());
       piece_list->setIsEnlargedPolygon(false);
       piece_list->setFixedSize(520, 520);
       this->ui->board_container->addWidget(piece_list , i / 3, i % 3);
       i++;
    }
}
