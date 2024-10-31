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

    bool empty() const;

private:
    void worker();

    std::vector<std::thread> workers;
    TaskQueue taskQueue;
    std::mutex queueMutex;
    std::condition_variable condition;

private:
    bool stop;
    std::function<void()> poolEmptyCallback; // Callback for pool empty
};
#endif //REPRODUCTIVE_SKEW_THREADPOOL_H
