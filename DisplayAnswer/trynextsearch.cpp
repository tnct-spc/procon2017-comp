#include "trynextsearch.h"
#include "ui_trynextsearch.h"

#include <QLayout>

TryNextSearch::TryNextSearch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TryNextSearch)
{
    ui->setupUi(this);

    board = new NeoAnswerBoard();
    board->setSingleMode(true);
    board->setSelectPieceMode(true);

    go_button = new QPushButton();
    go_button->setText("RUN BEAMSEARCH");

    connect(go_button,&QPushButton::clicked,this,&TryNextSearch::clickedGoButton);

    ui->verticalLayout->addWidget(board,0);
    ui->verticalLayout->addWidget(go_button,1);
}

TryNextSearch::~TryNextSearch()
{
    delete ui;
}

void TryNextSearch::setField(procon::NeoField field){
    this->field = field;
    board->setField(this->field);
}

void TryNextSearch::clickedGoButton()
{
    std::cout << "clicked go button" << std::endl;

    std::vector<int> piece_id = board->getSelectedPieceId();

    std::vector<polygon_i> polygons;
    for(auto id : piece_id){
        polygons.push_back(field.getPiece(id).getPolygon());
    }

    //完了したらwhile抜けるでしょ？
    bool complete_flag = false;

    //今は各領域一番最初のstepかな
    bool start_flag = true;

    polygon_i now_prosessing_polygon;

    //output
    std::vector<polygon_i> processed_frame;

    while(!complete_flag){
        //一つもピースがくっつけられなかったときに次の空間を作りに行くためのflag
        bool is_there_no_connect_piece = true;

        if(start_flag){
            start_flag = false;
            now_prosessing_polygon = polygons.at(0);
            polygons.erase(polygons.begin());
        }

        int index = 0;

        for(auto p : polygons){
            std::vector<polygon_i> out;

            boost::geometry::union_(now_prosessing_polygon,p,out);

            if(out.size() == 1){
                now_prosessing_polygon = out[0];
                is_there_no_connect_piece = false;

                //結合できたならpolygonsから除外していいよね
                polygons.erase(polygons.begin() + index);
            }

            ++index;
        }

        if(is_there_no_connect_piece){
            start_flag = true;
            processed_frame.push_back(now_prosessing_polygon);
        }

        if(polygons.empty()){
            complete_flag = true;
        }

    }

    procon::NeoField next_field;

    std::vector<procon::NeoExpandedPolygon> not_selected_pieces = field.getPieces();

    for(auto id : piece_id){
        auto ichi = std::find(not_selected_pieces.begin(),not_selected_pieces.end(),field.getPiece(id));
        not_selected_pieces.erase(ichi);
    }

    next_field.setElementaryPieces(field.getElementaryPieces());
    next_field.setElementaryFrame(field.getElementaryFrame());

    std::vector<procon::NeoExpandedPolygon> processed_expolygon;
    for(auto p : processed_frame){
        procon::NeoExpandedPolygon processed_expolygon_buf;
        processed_expolygon_buf.resetPolygonForce(p);
        processed_expolygon.push_back(processed_expolygon_buf);
    }

    next_field.setFrame(processed_expolygon);

    for(auto p : not_selected_pieces){
        next_field.setPiece(p);
    }

    emit startBeamSearch(this->field);
}
