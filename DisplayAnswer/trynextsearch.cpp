#include "trynextsearch.h"
#include "ui_trynextsearch.h"

#include "neopolygonviewer.h"

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
        for(auto p : field.getPieces()){
            if(id == p.getId()){
                polygons.push_back(p.getPolygon());
            }
        }
    }
    for(auto frame : field.getFrame()){
        polygons.push_back(frame.getPolygon());
    }

    std::vector<polygon_i> processed_frame;

    //connect wit frame
    while(true){
	    bool cannot_connect_piece = true;

        for(int index = 1; index < polygons.size(); ++index){

            std::vector<polygon_i> out;

            boost::geometry::union_(polygons[0],polygons[index],out);

		    if(out.size() == 1){
                cannot_connect_piece = false;

			    //結合できたならpolygonsから除外していいよね
                polygons.erase(polygons.begin() + index);
                polygons.erase(polygons.begin());

                //最後に結合したやつをまた突っ込む
                polygons.push_back(out[0]);
            }
        }

        if(polygons.empty()){
            break;
        }

        if(cannot_connect_piece){
            processed_frame.push_back(polygons[0]);
            polygons.erase(polygons.begin());
        }
    }

    procon::NeoField next_field;

    std::vector<procon::NeoExpandedPolygon> not_selected_pieces = field.getPieces();

    std::vector<int> delete_piece_index;
    for(int placed_piece_index = 0; placed_piece_index < not_selected_pieces.size(); ++placed_piece_index){
        auto index = std::find(piece_id.begin(),piece_id.end(),not_selected_pieces[placed_piece_index].getId());

        if(index != piece_id.end()){
//            not_selected_pieces.erase(not_selected_pieces.begin() + placed_piece_index);
            delete_piece_index.push_back(placed_piece_index);
        }
    }

    std::sort(delete_piece_index.begin(),delete_piece_index.end(),std::greater<int>());

    for(auto id : delete_piece_index){
        not_selected_pieces.erase(not_selected_pieces.begin() + id);
    }


    next_field.setIsPlaced(field.getIsPlaced());

    for(auto id : piece_id){
        next_field.setIsPlaced(id,false);
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

    NeoPolygonViewer::getInstance().displayPolygon(processed_expolygon.at(0).getPolygon(),"hgoe",false);

    NeoAnswerBoard *neoansboard = new NeoAnswerBoard();
    neoansboard->setSingleMode(true);
    neoansboard->setField(next_field);
    neoansboard->show();

    emit startBeamSearch(next_field);
}
