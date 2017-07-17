#pragma once

#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

namespace glamorous {

template <class T>
class threadsafe_queue {
private:
    mutable std::mutex mutex_;
    std::queue<T> queue_;
    std::condition_variable cond_data_;

public:
    threadsafe_queue() {}

    threadsafe_queue(const threadsafe_queue& other) {
        std::lock_guard<std::mutex> lock(other.mutex_);
        queue_ = other.queue_;
    }

    threadsafe_queue &operator=(const threadsafe_queue& other) {
        std::lock_guard<std::mutex> lock(other.mutex_);
        queue_ = other.queue_;
    }

    void push(T new_value) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(new_value);
        cond_data_.notify_one();
    }

    bool try_pop(T &value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return false;
        }
        value = queue_.front();
        queue_.pop();
        return true;
    }

    std::shared_ptr<T> try_pop() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> res(std::make_shared<T>(queue_.front()));
        queue_.pop();
        return res;
    }

    void wait_and_pop(T &value) {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_data_.wait(lock, [this]{return !queue_.empty();});
        value = queue_.front();
        queue_.pop();
    }

    std::shared_ptr<T> wait_and_pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_data_.wait(lock, [this]{return !queue_.empty();});
        std::shared_ptr<T> res(std::make_shared<T>(queue_.front()));
        queue_.pop();
        return res;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }
};

} // namespace glamorous
