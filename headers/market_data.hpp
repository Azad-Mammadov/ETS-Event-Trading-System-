// market_data.hpp
#pragma once

#include "event.hpp"        // Defines the Event class used to represent market or trading events
#include "event_queue.hpp"  // Provides the EventQueue class for managing and processing events
#include <thread>           // Enables multithreading for concurrent processing
#include <functional>       // Provides utilities for function objects and callbacks
#include <chrono>           // Provides utilities for time-related functions (e.g., delays)
#include <iostream>         // For standard input/output operations (e.g., std::cout)

#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <sstream>

// Callback function for cURL to write data into a string
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

// Simulated MarketDataFeed
class MarketDataFeed {
private:
    EventQueue<Event>* queue; // Pointer to the event queue for pushing market data events

public:
    // Constructor to initialize the market data feed with an event queue
    MarketDataFeed(EventQueue<Event>* q) : queue(q) {}

    // Starts the market data feed in a separate thread
    void start() {
        std::thread([this]() {
            while (true) {
                // Simulate market data
                Event e;
                e.symbol = "AAPL"; // Example stock symbol
                e.price = 150.0 + (std::rand() % 1000) / 100.0; // Random price simulation
                e.timestamp = "2025-04-08T12:00:00Z"; // Example timestamp

                queue->push(e); // Push the event to the queue
                std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate delay
            }
        }).detach(); // Detach the thread to run independently
    }
};


