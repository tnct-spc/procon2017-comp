#ifndef VECTOR2_H
#define VECTOR2_H
#include <iostream>

template <typename X>
class vector2{
    X x;
    X y;
public:
    vector2();
    vector2(X a,X b);
    X getx();
    X gety();
    void put(const X a,const X b);
    void print();
};


template<typename X>
vector2<X>::vector2(){
    this->x = 0;
    this->y = 0;
}

template<typename X>
vector2<X>::vector2(const X a,const X b){
    this->x = a;
    this->y = b;
}

template<typename X>
X vector2<X>::getx(){
    return this->x;
}

template<typename X>
X vector2<X>::gety(){
    return this->x;
}

template<typename X>
void vector2<X>::put(const X a,const X b){
    this->x = a;
    this->y = b;
}

template<typename X>
void vector2<X>::print(){
    std::cout << this->x << ' ' << this->y << std::endl;
}

#endif // VECTOR2_H
