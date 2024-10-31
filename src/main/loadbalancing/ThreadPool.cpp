#include <thread>

#include "ThreadPool.h"
#include "spdlog/spdlog.h"


ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(&ThreadPool::worker, this);
    }
}

ThreadPool::~ThreadPool() {
    {
        {
            std::unique_lock lock(queueMutex);
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
        std::function<void()> task; {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] { return stop || !taskQueue.empty(); });
            if (stop && taskQueue.empty()) return;
            task = taskQueue.pop();
        }
        task();
        std::unique_lock lock(queueMutex);
        if (taskQueue.empty() && poolEmptyCallback) {
            poolEmptyCallback(); // Call the callback when the pool is empty
            spdlog::trace("pool empty callback");
        }
    }
}




int ThreadPool::queueLength() const {
    return taskQueue.size();
}

bool ThreadPool::empty() const {
    return taskQueue.empty();
}
