#ifndef STRATEGY_HPP
#define STRATEGY_HPP

#include "event.hpp"
#include <functional>

class Strategy {
public:
    using OrderCallback = std::function<void(const Event&)>;

    void on_market_data(const Event& e);
    void set_order_callback(OrderCallback cb);

private:
    OrderCallback order_callback;
};

#endif // STRATEGY_HPP
