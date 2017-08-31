#ifndef NEOPOLYGONIO_H
#define NEOPOLYGONIO_H

#include "neofield.h"

class NeoPolygonIO
{
public:
    NeoPolygonIO();

    static void exportPolygon(procon::NeoField field,std::string file_path);
    static procon::NeoField importField(std::string file_path);

};

#endif // NEOPOLYGONIO_H
