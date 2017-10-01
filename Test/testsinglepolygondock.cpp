#include "testsinglepolygondock.h"

TestSinglePolygonDock::TestSinglePolygonDock()
{

}
bool TestSinglePolygonDock::run(){

    procon::NeoField field;
    SinglePolygonDock *dock = new SinglePolygonDock;
    dock->show();

    field = NeoPolygonIO::importField("../../procon2017-comp/field.csv");

    for(unsigned int count=0;count<field.getElementaryPieces().size();++count){
        if(!field.getIsPlaced().at(count)){
            dock->addPolygon(field.getElementaryPieces().at(count).getPolygon());
        }
    }

    return true;
}
