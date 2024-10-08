#include <iostream>
#include <thread>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#include "ThreadPool.h"

#include <condition_variable>
#include <utility>


ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(&ThreadPool::worker, this);
    }
}

ThreadPool::~ThreadPool() {
    {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread &worker: workers) {
            worker.join();
        }
    }
}



void ThreadPool::enqueue(std::function<void()> task) {
    taskQueue.push(std::move(task));
    condition.notify_one();
}

void ThreadPool::worker() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] { return stop || !taskQueue.empty(); });
            if (stop && taskQueue.empty()) return;
            task = taskQueue.pop();
        }
        task();


    }
}

int ThreadPool::queueLength() const {
    return taskQueue.size();
}

