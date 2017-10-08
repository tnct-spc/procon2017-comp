#include "findnoelement.h"
#include "neopolygonio.h"
#include "neopolygonviewer.h"
#include <QFileDialog>
#include <QString>

FindNoElement::FindNoElement()
{

}

bool FindNoElement::run(){

    std::string pieces_path = QFileDialog::getOpenFileName().toStdString();

    procon::NeoField field = NeoPolygonIO::importField(pieces_path);
    for(procon::NeoExpandedPolygon i : field.getElementaryPieces()){
        std::cout<<"-------------------------------"<<std::endl;
        for(point_i j : i.getPolygon().outer()){
            std::cout<<"x = "<<j.x()<<" , y = "<<j.y()<<std::endl;
        }
        NeoPolygonViewer::getInstance().displayPolygon(i.getPolygon(),"",false);
    }
    return true;
}
