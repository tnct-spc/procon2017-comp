#ifndef UTILITIES_H
#define UTILITIES_H

//#define HYOKA_MODE

#define TO_STRING(VariableName) # VariableName

#include "utilities_global.h"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

#define debugprint(S); std::cout<<"["<<__FILE__<<"]"<<"["<<std::to_string(__LINE__)<<"] "<<S<<std::endl;

namespace bg = boost::geometry;
using point_t = bg::model::d2::point_xy<double>;
using point_i = bg::model::d2::point_xy<int>;
using line_i = bg::model::linestring<point_i>;

class UTILITIESSHARED_EXPORT Utilities
{
private:
    Utilities();
public:
    template<typename X,typename Y>
    static X floor(X var,Y power)
    {
        return std::floor(var * std::pow(10,power)) / std::pow(10,power);
    }

    template<typename X,typename Y>
    static X round(X var,Y power)
    {
        return std::round(var * std::pow(10,power)) / std::pow(10,power);
    }

    template<typename X,typename Y>
    static bool nearlyEqual(X var1,X var2,Y allowance)
    {
        return std::abs(var1 - var2) < allowance ? true : false;
    }

    static bool cross_check(point_t a1, point_t a2, point_t b1, point_t b2){
        //tonari
        if((a1.x() == b1.x() && a1.y() == b1.y()) || (a1.x() == b2.x() && a1.y() == b2.y())) return true;
        double line1_a = (a2.y() - a1.y())/(a2.x() - a1.x());
        double line1_b = a2.x()-a1.x() > 0 ? a1.y()-line1_a*a1.x() : a2.y()-line1_a*a2.x();
        double line2_a = (b2.y() - b1.y())/(b2.x() - b1.x());
        double line2_b = b2.x()-b1.x() > 0 ? b1.y()-line2_a*b1.x() : b2.y()-line2_a*b2.x();

        //2本の線を直線としたときの交点のX座標
        double cross_pointX = (line2_b - line1_b)/(line1_a - line2_a);

        //X座標でくらべて、線どうしが交差しているかを判定
        bool cr1 = false;
        bool cr2 = false;
        if(a1.x() < cross_pointX){
            if(a1.x() < cross_pointX && a2.x() > cross_pointX) cr1 = true;
        }else{
            if(a2.x() < cross_pointX && a1.x() > cross_pointX) cr1 = true;
        }
        if(b1.x() < cross_pointX){
            if(b1.x() < cross_pointX && b2.x() > cross_pointX) cr2 = true;
        }else{
            if(b2.x() < cross_pointX && b1.x() > cross_pointX) cr2 = true;
        }
        return cr1 && cr2;
    }

    static bool cross_check(point_i a1, point_i a2, point_i b1, point_i b2){
        line_i line1{a1, a2}, line2{b1, b2};
        return bg::intersects(line1, line2);
    }

    static int inc(int num,int size, int len=1){
        return (num+len) % size;
    }
    static int dec(int num,int size, int len=1){
        return ((num-len)%size + size) % size;
    }

    // Not supported on c++11
    template<typename T, typename ...Args>
    static std::unique_ptr<T> make_unique( Args&& ...args )
    {
        return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
    }
};
#endif // UTILITIES_H
