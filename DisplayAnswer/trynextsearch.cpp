#include "trynextsearch.h"
#include "ui_trynextsearch.h"

#include "neopolygonviewer.h"

#include <QLayout>
#include <QGraphicsView>

TryNextSearch::TryNextSearch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TryNextSearch)
{
    ui->setupUi(this);

    board = new NeoAnswerBoard();
    board->setSingleMode(true);

    dock = new SinglePolygonDock();


//    board->setShowUnplacedPieces(true);
    board->setSelectPieceMode(true);
    board->setZoomMode(true);

    go_button = new QPushButton();
    go_button->setText("RUN BEAMSEARCH");

    connect(go_button,&QPushButton::clicked,this,&TryNextSearch::clickedGoButton);


    QGraphicsScene *scene = new QGraphicsScene();
    view = new QGraphicsView();

    scene->addWidget(board);
    view->setScene(scene);
    view->scale(zoom,zoom);
    ui->gridLayout->addWidget(view,0,0,1,10);

  //  ui->gridLayout->addWidget(board,0,0,1,10);
    ui->gridLayout->addWidget(dock,0,10,1,2);
    ui->gridLayout->addWidget(go_button,1,0,1,0);

    connect(this->board,SIGNAL(zoom_In()),this,SLOT(_zoom_In()));
    connect(this->board,SIGNAL(zoom_Out()),this,SLOT(_zoom_Out()));

}

TryNextSearch::~TryNextSearch()
{
    delete ui;
}

void TryNextSearch::setField(procon::NeoField field){
    this->field = field;
    board->setField(this->field);

    for(unsigned int count=0;count<field.getElementaryPieces().size();++count){
        if(!field.getIsPlaced().at(count))dock->addPolygon(field.getElementaryPieces().at(count).getPolygon());
    }
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

//#define DEBUG
#ifdef DEBUG
    NeoAnswerBoard *neoansboard = new NeoAnswerBoard();
    neoansboard->setSingleMode(true);
    neoansboard->setField(next_field);
    neoansboard->show();
#endif


    if(checkFloatPiece(processed_frame))std::cout << "エラー！！！！" << std::endl; //ここでエラー吐いた時の処理(QMessageBoxとかloggerとか)をやる
    else{

    emit startBeamSearch(next_field);

    this->close();
    }
}

bool TryNextSearch::checkFloatPiece(std::vector<polygon_i> frames){

    for(auto frame : frames){
        if(frame.inners().size()!=0)return true;
    }

    return false;//問題ないならfalse
}

void TryNextSearch::_zoom_In(){
    view->update();
    double value = zoom+0.5;
    if(!(value>99 || value<0.5)){
    double zoom_ = value / zoom;
    zoom = value;
    view->scale(zoom_,zoom_);
    view->update();
    }
}

void TryNextSearch::_zoom_Out(){
    double value = zoom-0.5;
    if(!(value>99 || value<0.5)){
    double zoom_ = value / zoom;
    zoom = value;
    view->scale(zoom_,zoom_);
    view->update();
    }
}
