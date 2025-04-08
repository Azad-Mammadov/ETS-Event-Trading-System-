#ifndef STRATEGY_HPP
#define STRATEGY_HPP

#include "event.hpp"
#include <functional>
#include <string>

class Strategy {
public:
    using OrderCallback = std::function<void(const Event&)>;
    using LogCallback = std::function<void(const std::string&)>;

    void set_order_callback(OrderCallback callback);
    void set_logger_callback(LogCallback callback);
    void on_market_data(const Event& e);

private:
    OrderCallback order_callback;
    LogCallback log_callback;
};

#endif // STRATEGY_HPP
