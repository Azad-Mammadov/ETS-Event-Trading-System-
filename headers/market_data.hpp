
#ifndef MARKET_DATA_HPP
#define MARKET_DATA_HPP

#include "event.hpp"
#include <string>
#include <vector>
#include <functional>

class MarketDataFeed {
public:
    using Callback = std::function<void(const Event&)>;

    void subscribe(Callback callback);
    void start();

private:
    std::vector<Callback> subscribers;
    void emit_price_update(const std::string& symbol, double price);
};

#endif // MARKET_DATA_HPP

