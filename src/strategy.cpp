
#include "strategy.hpp"
#include <iostream>
#include <thread>
#include <chrono>

void Strategy::on_event(Event& event) {
    std::cout << "[Strategy] Handling event: " << event.content << std::endl;
}

void Strategy::run(std::queue<Event>& event_queue, std::mutex& queue_mutex) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        std::lock_guard<std::mutex> lock(queue_mutex);
        if (!event_queue.empty()) {
            Event event = event_queue.front();
            event_queue.pop();
            on_event(event);
        }
    }
}

