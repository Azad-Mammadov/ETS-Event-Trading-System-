
#include "headers\market_data.hpp"
#include <thread>
#include <chrono>
#include <random>

void MarketDataFeed::subscribe(Callback callback) {
    subscribers.push_back(callback);
}

void MarketDataFeed::start() {
    std::vector<std::string> symbols = {"AAPL", "GOOG", "TSLA"};
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(100.0, 200.0);

    for (int i = 0; i < 10; ++i) {
        for (const auto& symbol : symbols) {
            double price = distribution(generator);
            emit_price_update(symbol, price);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
}

void MarketDataFeed::emit_price_update(const std::string& symbol, double price) {
    Event e(EventType::MarketData);
    e.symbol = symbol;
    e.price = price;
    for (const auto& cb : subscribers) {
        cb(e);
    }
}

