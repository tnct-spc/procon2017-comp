#include "testsinglepolygondock.h"

TestSinglePolygonDock::TestSinglePolygonDock()
{

}
bool TestSinglePolygonDock::run(){

    procon::NeoField field;
    SinglePolygonDock *dock = new SinglePolygonDock;
    TryNextSearch *search = new TryNextSearch;
    field = NeoPolygonIO::importField("../../../humanpowerfield.csv");

   // dock->show();
    search->show();


    search->setField(field);

    for(unsigned int count=0;count<field.getElementaryPieces().size();++count){
        if(!field.getIsPlaced().at(count)){
            dock->addPolygon(field.getElementaryPieces().at(count).getPolygon());
        }
    }



    return true;
}
