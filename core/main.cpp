#include "market_data.hpp"  // Handles market data feed and provides market-related events
#include "strategy.hpp"     // Defines the trading strategy logic and callbacks
#include "order.hpp"        // Manages order creation and execution
#include "risk.hpp"         // Implements risk management for validating orders
#include "logger.hpp"       // Provides logging functionality for system events
#include "portfolio.hpp"    // Manages portfolio positions and calculates portfolio value
#include "event_queue.hpp"  // Provides a thread-safe event queue for communication between components
#include <thread>           // Enables multithreading for concurrent processing

int main() {
    EventQueue<Event> eventQueue;
    MarketDataFeed feed(&eventQueue);
    Strategy strategy;
    OrderExecution executor;
    RiskManager risk;
    PortfolioManager portfolio;

    strategy.set_order_callback([&](const Event& order_event) {
        if (risk.validate_order(order_event)) {
            executor.execute_order(order_event);
            Logger::log("Order executed for: " + order_event.symbol);
            portfolio.update_position(order_event.symbol, 10, order_event.price);
        }
    });

    strategy.set_logger_callback([](const std::string& msg) {
        Logger::log(msg);
    });

    feed.start(); // starts producer thread

    // Consumer thread that passes data to the strategy
    std::thread processor([&]() {
        while (true) {
            Event e = eventQueue.pop();
            strategy.on_market_data(e);
        }
    });

    // Let the system run
    processor.join();

    Logger::log("Final Portfolio Value: " + std::to_string(portfolio.get_value()));
    return 0;
}
