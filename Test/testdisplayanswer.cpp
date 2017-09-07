#include "testdisplayanswer.h"
#include "neoanswerboard.h"

TestDisplayAnswer::TestDisplayAnswer()
{
}

bool TestDisplayAnswer::run()
{
    NeoAnswerBoard *nab = new NeoAnswerBoard();
    polygon_i boost_polygon;
    bg::exterior_ring ( boost_polygon ) = boost::assign::list_of < point_i >
            (0,0) (0,30) (60,30) (60,0) (0,0);
    procon::NeoExpandedPolygon nep;
    nep.resetPolygonForce(boost_polygon);
    procon::NeoField neofield;
    neofield.setElementaryFrame({nep});
    nab->setField(neofield);
    nab->setText("hello");
    nab->setSingleMode(true);
    nab->show();
    return true;
}
