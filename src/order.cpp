
#include "order.hpp"
#include <iostream>

void OrderExecution::execute_order(const Event& order_event) {
    std::cout << "Executing Order: " << order_event.symbol
              << " at price: " << order_event.price << std::endl;
}

// This function simulates order execution logic. In a real-world scenario, this would involve