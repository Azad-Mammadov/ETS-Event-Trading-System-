#include "headers\market_data.hpp"   // Declares the MarketDataFeed class and its methods
#include "headers\event_queue.hpp"   // Provides the EventQueue class for managing and processing events
#include "headers\logger.hpp"        // Provides logging functionality for system events

#include <curl/curl.h>               // cURL library for making HTTP requests
#include <nlohmann/json.hpp>         // JSON library for parsing and handling JSON data
#include <thread>                    // Enables multithreading for concurrent processing
#include <chrono>                    // Provides utilities for time-related functions (e.g., delays)
#include <ctime>                     // Provides utilities for working with time (e.g., timestamps)

using json = nlohmann::json;         // Alias for easier usage of the JSON library

// Callback function for cURL to write data into a string
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

// Subscribes to market data updates with a callback function
void MarketDataFeed::subscribe(std::function<void(const Event&)> callback) {
    this->callback = callback;
}

// Starts the market data feed in a separate thread
void MarketDataFeed::start() {
    std::string api_key = "cvqogt9r01qp88cms3i0cvqogt9r01qp88cms3ig"; // API key for authentication
    std::string symbol = "AAPL";                                      // Stock symbol to fetch data for

    std::thread([=]() {
        CURL* curl = curl_easy_init();        // Initialize cURL
        curl_global_init(CURL_GLOBAL_ALL);   // Global cURL initialization

        while (true) {
            std::string readBuffer;
            std::string url = "https://finnhub.io/api/v1/quote?symbol=" + symbol + "&token=" + api_key;

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());               // Set the URL
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);  // Set the write callback
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);        // Set the buffer to store response

            CURLcode res = curl_easy_perform(curl); // Perform the HTTP request
            if (res == CURLE_OK) {
                try {
                    auto data = json::parse(readBuffer); // Parse the JSON response
                    if (data.contains("c") && data["c"].is_number()) {
                        Event e;
                        e.symbol = symbol;
                        e.price = data["c"];
                        e.timestamp = std::to_string(std::time(nullptr));
                        Logger::log("Received: " + e.symbol + " $" + std::to_string(e.price));
                        if (callback) callback(e);  // Invoke the subscription callback
                    }
                } catch (const std::exception& ex) {
                    Logger::log("JSON Parse Error: " + std::string(ex.what()));
                }
            } else {
                Logger::log("CURL Error: " + std::string(curl_easy_strerror(res)));
            }

            std::this_thread::sleep_for(std::chrono::seconds(10)); // Wait before the next request
        }

        curl_easy_cleanup(curl);       // Clean up cURL resources
        curl_global_cleanup();         // Global cleanup for cURL
    }).detach(); // Detach the thread to run independently
}
