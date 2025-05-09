// market_data.hpp
#pragma once

#include "event.hpp"        // Defines the Event class used to represent market or trading events
#include "event_queue.hpp"  // Provides the EventQueue class for managing and processing events
#include <thread>           // Enables multithreading for concurrent processing
#include <functional>       // Provides utilities for function objects and callbacks
#include <chrono>           // Provides utilities for time-related functions (e.g., delays)
#include <iostream>         // For standard input/output operations (e.g., std::cout)

#include <curl/curl.h> // Ensure cURL is installed and the include path is configured
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
            CURL* curl = curl_easy_init();
            std::string api_key = "cvqogt9r01qp88cms3i0cvqogt9r01qp88cms3ig";
            std::string symbol = "AAPL";
    
            while (true) {
                if (curl) {
                    std::string readBuffer;
                    std::stringstream url;
                    url << "https://finnhub.io/api/v1/quote?symbol=" << symbol << "&token=" << api_key;
    
                    curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    
                    CURLcode res = curl_easy_perform(curl);
                    if (res == CURLE_OK) {
                        try {
                            auto data = nlohmann::json::parse(readBuffer);
    
                            Event e;
                            e.symbol = symbol;
                            e.price = data["c"];  // current price
                            e.timestamp = std::to_string(std::time(nullptr));
    
                            queue->push(e);
                        } catch (...) {
                            std::cerr << "Failed to parse JSON" << std::endl;
                        }
                    } else {
                        std::cerr << "CURL failed: " << curl_easy_strerror(res) << std::endl;
                    }
    
                    std::this_thread::sleep_for(std::chrono::seconds(10)); // limit hits!
                }
            }
    
            curl_easy_cleanup(curl);
        }).detach();
    }
    
};


