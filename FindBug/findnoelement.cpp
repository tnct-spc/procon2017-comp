#include "findnoelement.h"
#include "neopolygonio.h"

FindNoElement::FindNoElement()
{

}

bool FindNoElement::run(){
    procon::NeoField field = NeoPolygonIO::importField("/home/yui/Procon/fieldcdv/");
    nab = std::make_shared<NeoAnswerBoard>();
    nab->setField(field);
    nab->show();
    return true;
}
