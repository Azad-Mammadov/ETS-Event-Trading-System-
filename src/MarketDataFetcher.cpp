#include "MarketDataFetcher.hpp" // Declares the fetch_market_data function and related functionality
#include <curl/curl.h>           // cURL library for making HTTP requests .
#include <nlohmann/json.hpp>     // JSON library for parsing and handling JSON data
#include <chrono>                // Provides utilities for time-related functions (e.g., delays)
#include <thread>                // Enables multithreading for concurrent processing

using json = nlohmann::json;     // Alias for easier usage of the JSON library

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

void fetch_market_data(const std::string& symbol, const std::string& api_key, ThreadSafeQueue<Event>& queue) {
    CURL* curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    while (true) {
        if (curl) {
            std::string readBuffer;
            std::string url = "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=" + symbol + "&apikey=" + api_key;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);

            if (res == CURLE_OK) {
                auto data = json::parse(readBuffer);
                auto quote = data["Global Quote"];
                if (!quote.empty()) {
                    Event e;
                    e.symbol = quote["01. symbol"].get<std::string>();
                    e.price = std::stod(quote["05. price"].get<std::string>());
                    e.timestamp = quote["07. latest trading day"].get<std::string>();
                    queue.push(e);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(20));
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();
}
