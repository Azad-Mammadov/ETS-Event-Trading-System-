#include "strategy.hpp"

void Strategy::set_order_callback(OrderCallback callback) {
    order_callback = callback;
}

void Strategy::set_logger_callback(LogCallback callback) {
    log_callback = callback;
}

void Strategy::on_market_data(const Event& e) {
    if (e.price < 150) {
        Event order(EventType::Order);
        order.symbol = e.symbol;
        order.price = e.price;

        if (order_callback) {
            order_callback(order);
        }

        if (log_callback) {
            log_callback("Strategy triggered order for: " + e.symbol + " at price " + std::to_string(e.price));
        }
    }
}
