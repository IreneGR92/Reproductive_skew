
#include "TaskQueue.h"

void TaskQueue::push(std::function<void()> task) {
    std::unique_lock<std::mutex> lock(mutex_);
    tasks_.push(std::move(task));
    condition_.notify_one();
}

std::function<void()> TaskQueue::pop() {
    std::unique_lock<std::mutex> lock(mutex_);
    condition_.wait(lock, [this] { return !tasks_.empty(); });
    auto task = std::move(tasks_.front());
    tasks_.pop();
    return task;
}

bool TaskQueue::empty() const {
    std::unique_lock<std::mutex> lock(mutex_);
    return tasks_.empty();
}

int TaskQueue::size() const {
    std::unique_lock<std::mutex> lock(mutex_);
    return tasks_.size();
}
