
#ifndef REPRODUCTIVE_SKEW_TASKQUEUE_H
#define REPRODUCTIVE_SKEW_TASKQUEUE_H


#include <functional>
#include <mutex>
#include <queue>
#include <condition_variable>

class TaskQueue {
public:
    void push(std::function<void()> task);

    std::function<void()> pop();

    bool empty() const;

    int size() const;

private:
    std::queue<std::function<void()>> tasks_;
    mutable std::mutex mutex_;
    std::condition_variable condition_;
};

#endif //REPRODUCTIVE_SKEW_TASKQUEUE_H
