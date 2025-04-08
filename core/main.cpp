#include "strategy.hpp" // Custom header defining the Strategy class and related functionality
#include <iostream>     // For input/output operations (e.g., std::cout)
#include <queue>        // For using the std::queue container to manage events
#include <mutex>        // For thread-safe access to shared resources (e.g., std::mutex, std::lock_guard)
#include <thread>       // For creating and managing threads (e.g., std::thread)
#include <chrono>       // For time-related utilities (e.g., std::this_thread::sleep_for)

int main() {
    std::queue<Event> event_queue;
    std::mutex queue_mutex;

    Strategy strategy;
    std::thread strategy_thread(&Strategy::run, &strategy, std::ref(event_queue), std::ref(queue_mutex));

    // Simulate market data feed
    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        std::lock_guard<std::mutex> lock(queue_mutex);
        event_queue.push(Event(EventType::MARKET, "Price tick " + std::to_string(100 + i)));
        std::cout << "[Main] Event added to queue\n";
    }

    strategy_thread.detach(); // Let strategy thread run independently
    std::this_thread::sleep_for(std::chrono::seconds(3)); // Let it process some events

    std::cout << "[Main] Exiting...\n";
    return 0;
}

