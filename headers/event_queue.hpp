// event_queue.hpp
#pragma once

#include <queue>                // Provides the std::queue container for managing a queue of events
#include <mutex>                // Provides std::mutex for thread-safe access to shared resources
#include <condition_variable>   // Provides std::condition_variable for thread synchronization

// Template class for a thread-safe event queue
template<typename T>
class EventQueue {
private:
    std::queue<T> queue;                // Queue to store events
    std::mutex mtx;                     // Mutex to ensure thread-safe access to the queue
    std::condition_variable cv;         // Condition variable for thread synchronization

public:
    // Pushes an item into the queue and notifies waiting threads
    void push(const T& item) {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(item);
        cv.notify_one();
    }

    // Pops an item from the queue, blocking if the queue is empty
    T pop() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&] { return !queue.empty(); });
        T item = queue.front();
        queue.pop();
        return item;
    }

    // Checks if the queue is empty
    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return queue.empty();
    }
};

