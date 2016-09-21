#ifndef POLYGONIO_H
#define POLYGONIO_H

#include "field.h"

namespace procon {
class PolygonIO
{
public:
    PolygonIO();
    static procon::Field importPolygon(std::string file_path);
    static void exportPolygon(procon::Field field, std::string file_path);
    static procon::Field importAnswer(std::string file_path, procon::Field field);
    static void exportAnswer(procon::Field field, std::string file_path);
};

}
#endif // POLYGONIO_H
