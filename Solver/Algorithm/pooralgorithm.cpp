#include "pooralgorithm.h"

PoorAlgorithm::PoorAlgorithm()
{

}

Field PoorAlgorithm::run(PolygonSet set)
{
    Field field;

    field.setFlame(set.fieldFlame);
    field.pushPiece(set.fieldPiece.at(0),15,15);

    return field;
}
