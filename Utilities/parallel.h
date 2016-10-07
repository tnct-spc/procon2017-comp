#ifndef PARALLEL_H
#define PARALLEL_H

#include <thread>
#include <mutex>

#define MUTEX_LOCK(PARALLEL) volatile std::lock_guard<std::mutex> lg(PARALLEL.mutex())

namespace procon {
class Parallel
{
private:
    std::vector<std::thread> threads;
    std::mutex _mutex;

public:
    Parallel();
    template <typename Lambda,typename Split,typename Range>
    void generateThreads(Lambda lambda,Split split,Range start,Range end) {
        threads.clear();
        Split split_num = (end - start) / split;
        Range i = 0;
        if (split_num > 0) {
            for (i = 0;i < split - 1;i++) {
                std::thread thread(lambda,start + split_num * i,start + split_num * (i + 1));
                threads.emplace_back(std::move(thread));
            }
        }
        std::thread thread(lambda,start + split_num * i,end);
        threads.emplace_back(std::move(thread));
    }

    void joinThreads() {
        for (auto & thread : threads) {
            thread.join();
        }
    }

    std::mutex & mutex() {
        return _mutex;
    }

};
}
#endif // PARALLEL_H
