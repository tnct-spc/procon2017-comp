#include "complexangle.h"
#include <functional>

ComplexAngle::ComplexAngle(int x, int y)
{
    if((x == 0) || (y == 0)) {
        x = (y == 0) ? ((std::signbit(x)) ? -1 : 1) : 0;
        y = (x == 0) ? ((std::signbit(y)) ? -1 : 1) : 0;
    } else {
        std::function<int(int, int)> gcd = [&gcd](int a, int b)
        {
            return b ? gcd(b, a % b) : a;
        };

        int a = gcd(std::abs(x), std::abs(y));
        x /= a;
        y /= a;
    }
    this->x = x;
    this->y = y;
}

ComplexAngle ComplexAngle::operator /(ComplexAngle angle)
{
    return ComplexAngle((this->x * angle.x) + (this->y * angle.y), (this->y * angle.x) - (this->x * angle.y));
}

ComplexAngle ComplexAngle::operator =(ComplexAngle angle)
{
    this->x = angle.x;
    this->y = angle.y;
    return *this;
}

bool ComplexAngle::operator ==(ComplexAngle angle)
{
    double x1 = this->x;
    double y1 = this->y;
    double x2 = angle.x;
    double y2 = angle.y;
    if(((y1 == 0.0) && (y2 == 0.0)) && (std::signbit(x1) == std::signbit(x2))) return true;
    else if((x1 / y1 == x2 / y2) && (std::signbit(x1) == std::signbit(x2)) && (std::signbit(y1) == std::signbit(y2))) return true;
    else return false;
}
