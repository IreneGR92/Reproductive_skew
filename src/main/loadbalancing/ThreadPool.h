
#ifndef REPRODUCTIVE_SKEW_THREADPOOL_H
#define REPRODUCTIVE_SKEW_THREADPOOL_H


#include <functional>
#include <queue>
#include <condition_variable>
#include "TaskQueue.h"

class ThreadPool {
public:
    explicit ThreadPool(size_t numThreads);

    ~ThreadPool();

    void enqueue(std::function<void()> task);

    int queueLength() const;


private:
    void worker();

    std::vector<std::thread> workers;
    TaskQueue taskQueue;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;
};

#endif //REPRODUCTIVE_SKEW_THREADPOOL_H
