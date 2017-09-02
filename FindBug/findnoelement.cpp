#include "findnoelement.h"
#include "neopolygonio.h"
#include "neopolygonviewer.h"

FindNoElement::FindNoElement()
{

}

bool FindNoElement::run(){
    procon::NeoField field = NeoPolygonIO::importField("/home/yui/Procon/fieldcdv/made_field5.csv");
    for(procon::NeoExpandedPolygon i : field.getElementaryPieces()){
        std::cout<<"-------------------------------"<<std::endl;
        for(point_i j : i.getPolygon().outer()){
            std::cout<<"x = "<<j.x()<<" , y = "<<j.y()<<std::endl;
        }
        NeoPolygonViewer::getInstance().displayPolygon(i.getPolygon(),"",false);
    }
    return true;
}
