
#include "MarketDataFetcher.hpp"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <chrono>
#include <thread>

using json = nlohmann::json;

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


// This code fetches market data from the Alpha Vantage API for a given symbol and pushes the data into a thread-safe queue. It uses libcurl for HTTP requests and nlohmann/json for JSON parsing. The function runs in an infinite loop, fetching data every 20 seconds.
// The `fetch_market_data` function takes a symbol and an API key as input, constructs the API URL, and retrieves the market data. It parses the JSON response to extract the symbol, price, and timestamp, and then pushes this data into a thread-safe queue for further processing by other components of the trading system. The function uses a callback to handle the HTTP response and a loop to continuously fetch data at regular intervals.
// The code is designed to be modular and can be easily integrated into a larger trading system. It can be extended to handle different symbols or data sources as needed.
// The `fetch_market_data` function is designed to run indefinitely, continuously fetching market data at specified intervals. It uses a thread-safe queue to ensure that the data can be safely accessed by multiple threads in the trading system. The function can be easily modified to include error handling, logging, or other features as needed for a production environment.
// The code is structured to be easily extendable, allowing for the addition of new features or strategies in the future. Each component can be modified or replaced independently, making it a flexible foundation for a trading system.
