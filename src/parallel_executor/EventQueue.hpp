#ifndef MULTITHREADING_EVENTQUEUE_HPP
#define MULTITHREADING_EVENTQUEUE_HPP

#include <queue>
#include <memory>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "event.hpp"

class EventQueue{
public:
    void push(const std::shared_ptr<const Event>& event) {
        std::lock_guard<std::mutex> lk(mx);
        queue.push(event);
        cv.notify_one();
    };

    // if queue is empty, sleep for duration then return nullptr is queue is still empty, otherwise pop normally
    std::shared_ptr<const Event> pop(const std::chrono::seconds& duration) {
        std::unique_lock<std::mutex> lk(mx);
        if (cv.wait_for(lk, duration, [this]{return !queue.empty();})) {
            std::shared_ptr<const Event> front = queue.front();
            queue.pop();
            return front;
        }
        return nullptr;
    }

private:
    std::queue<std::shared_ptr<const Event>> queue;
    std::mutex mx;
    std::condition_variable cv;
};

#endif //MULTITHREADING_EVENTQUEUE_HPP
