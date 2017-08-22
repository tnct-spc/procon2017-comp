#include <iostream>
#include <chrono>
#include <random>
#include <thread>
#include <mutex>

#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

#include <tbb/task_scheduler_init.h>
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>

namespace bg = boost::geometry;

typedef bg::model::d2::point_xy<int> point;
typedef bg::model::box<point> box;

std::mutex mtx;
std::vector<int> processed;

template<typename T,typename Lambda>
void Prallel(std::vector<T> const& values,unsigned int thread_num,Lambda lambda){
    //タスク分割
    for (unsigned int index = 0; index < thread_num; ++index) {
        unsigned int divided_size = static_cast<int>(values.size() / thread_num);

    }


}

int main(int argc, char *argv[])
{
    std::cout << "Hello World!" << std::endl;

    const box a(point(0, 0), point(3, 3));
    const box b(point(4, 4), point(7, 7));

    auto start = std::chrono::system_clock::now();
    for (int c = 0; c < 10000; ++c) {
        bool result = bg::disjoint(a, b);
    }
    auto end = std::chrono::system_clock::now();

    std::cout << "disjoint 競合判定時間" << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;

    std::vector<int> hoge = { 1,5,3,8,10,19,5,1 };

    for(auto const& v : hoge) std::cout << v << ",";
    std::cout << std::endl;

    std::sort(hoge.begin(),hoge.end(),[](int r,int l){
        return r > l;
    });

    std::vector<int> test;
    std::random_device rnd;
    for (int d = 0; d < 1000000; ++d) {
        test.push_back(rnd());
    }

    std::vector<std::thread> threads(100);
    for(auto& th : threads){
//        th = std::
    }

    return 0;
}
