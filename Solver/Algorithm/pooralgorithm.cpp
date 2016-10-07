#include "pooralgorithm.h"

#include "singlepolygondisplay.h"

PoorAlgorithm::PoorAlgorithm()
{

}

void PoorAlgorithm::run(procon::Field field)
{
    field.setFrame(field.getElementaryFrame());
    field.setPiece(field.getElementaryPieces().at(0),15.0,15.0);
    procon::ExpandedPolygon p = field.getElementaryPieces().at(0);
    p.rotatePolygon(45);
    field.setPiece(p,20.0,20.0);
    DOCK->addAnswer(field);
    submitAnswer(field);
    return;
}

