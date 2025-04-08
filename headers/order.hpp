#ifndef ORDER_HPP
#define ORDER_HPP

#include "event.hpp"  // Defines the Event class used to represent market or trading events
#include <string>     // Provides the std::string class for handling text data

// Class responsible for executing orders
class OrderExecution {
public:
    // Executes an order based on the provided event
    void execute_order(const Event& order_event);
};

#endif // ORDER_HPP

