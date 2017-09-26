#include "testdisplayanswer.h"
#include "neoanswerboard.h"
#include "neoanswerdock.h"

TestDisplayAnswer::TestDisplayAnswer()
{
}

bool TestDisplayAnswer::run()
{
    NeoAnswerDock *nad = new NeoAnswerDock();
    polygon_i boost_polygon;
    bg::exterior_ring ( boost_polygon ) = boost::assign::list_of < point_i >
            (0,0) (0,30) (60,30) (60,0) (0,0);
    procon::NeoExpandedPolygon nep;
    nep.resetPolygonForce(boost_polygon);
    procon::NeoField neofield;
    neofield.setElementaryFrame({nep});

    NeoAnswerBoard *nab = new NeoAnswerBoard();
    nab->setField(neofield);
    nab->setText("hello");
    nab->show();

    nad->addAnswer(neofield , "test");
    nad->addAnswer(neofield , "aaaaa");
    nad->addAnswer(neofield , "hello");
    nad->addAnswer(neofield);
    nad->addAnswer(neofield);
    nad->addAnswer(neofield);
    nad->addAnswer(neofield);
    nad->addAnswer(neofield);
    nad->show();
    return true;
}
