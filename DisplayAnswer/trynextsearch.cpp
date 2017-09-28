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

    std::vector<polygon_i> processed_pieces;

    //connect wit frame
    while(true){
	    bool cannot_connect_piece = true;

        for(int index = 1; index < polygons.size(); ++index){

            std::vector<polygon_i> out;

            boost::geometry::union_(polygons[0],polygons[index],out);

		    if(out.size() == 1){
			    now_prosessing_polygon = out[0];
			    cannot_connect_piece = false;

			    //結合できたならpolygonsから除外していいよね
                polygons.erase(polygons.begin() + index);
                polygons.erase(polygons.begin());
            }
        }

        if(polygons.empty()){
            break;
        }

        if(cannot_connect_piece){
            processed_pieces.push_back(polygons[0]);
        }
    }

    procon::NeoField next_field;

    std::vector<procon::NeoExpandedPolygon> not_selected_pieces = field.getPieces();

    for(auto id : piece_id){
        auto ichi = std::find(not_selected_pieces.begin(),not_selected_pieces.end(),field.getElementaryPieces().at(id));

        not_selected_pieces.erase(ichi);

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
