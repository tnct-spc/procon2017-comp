#include "testqrtranslatetopolygon.h"

TestQrTranslateToPolygon::TestQrTranslateToPolygon()
{
    std::string str = "15:9 0 0 26 0 26 5 18 5 18 17 15 24 10 24 4 20 0 20:6 0 0 4 0 10 4 9 8 4 6 0 6:6 0 0 4 0 9 2 8 6 6 16 0 16:11 0 0 6 0 5 4 4 12 24 18 34 12 38 9 38 17 46 17 46 22 0 22:5 1 0 8 2 17 3 20 14 0 8:5 1 5 2 0 7 2 7 11 0 9:5 1 0 10 0 12 8 5 7 0 5:4 2 0 7 0 9 8 0 8:4 0 0 7 1 9 10 0 9:9 0 12 3 5 11 0 27 0 38 7 37 12 27 6 11 6 1 16:6 10 0 19 0 12 6 10 10 1 14 0 10:5 0 4 9 0 12 7 6 11 2 12:9 0 5 8 5 8 0 32 0 32 7 24 7 24 12 8 12 0 17:6 0 5 4 4 10 0 14 7 6 11 2 14:5 0 5 4 2 8 0 13 10 3 16";
    std::cout << str << std::endl;
    QrTranslateToPolygon QrTrans(str);
}
